using System.Drawing;
using System.Net.Http.Headers;
using System.Windows.Forms;
using System.Xml.Serialization;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace ImagesRgbToCmykConverter
{
    public partial class CMYKForm : Form
    {
        public Image? mainImage;
        public Bitmap? mainImageBitmap;
        public Bitmap bezierPlotsBitmap;
        public List<List<Point>> bezierControlPoints;
        public List<Color> colorList;
        public int ActiveControlPoint = -1;
        public List<List<Point>> bezierCurves;

        public int ActiveIndex { get; set; }

        public List<Point> CubicBezierToPoints(Point P0, Point P1, Point P2, Point P3, double step = 0.01)
        {
            var pointList = new List<Point>();
            for (var t = 0.00; t <= 1; t += step)
            {
                var x = Math.Pow(1 - t, 3) * P0.X + 3 * Math.Pow(1 - t, 2) * t * P1.X +
                        3 * (1 - t) * Math.Pow(t, 2) * P2.X + Math.Pow(t, 3) * P3.X;
                var y = Math.Pow(1 - t, 3) * P0.Y + 3 * Math.Pow(1 - t, 2) * t * P1.Y +
                        3 * (1 - t) * Math.Pow(t, 2) * P2.Y + Math.Pow(t, 3) * P3.Y;
                pointList.Add(new Point((int)x, (int)y));
            }
            return pointList;
        }

        public List<List<Point>> GetBezierCurves()
        {
            List<List<Point>> result = new List<List<Point>>();

            foreach(var controls in bezierControlPoints)
            {
                result.Add(CubicBezierToPoints(controls[0], controls[1],
                    controls[2], controls[3]));
            }
            return result;
        }

        public void DrawLine(int index, Color color)
        {
            Graphics gBmp = Graphics.FromImage(bezierPlotsBitmap);
            gBmp.DrawBeziers(new Pen(color), bezierControlPoints[index].ToArray());
            if (index != ActiveIndex)
                return;

            foreach (var point in bezierControlPoints[index])
                gBmp.DrawEllipse(new Pen(Color.Black),
                    point.X - 5, point.Y - 5, 10, 10);
        }

        public void DrawPlot()
        {
            bezierPlotsBitmap = new Bitmap(BezierPlot.Width, BezierPlot.Height);
            BezierPlot.Image = bezierPlotsBitmap;
            Graphics gBmp = Graphics.FromImage(bezierPlotsBitmap);
            gBmp.FillRectangle(new SolidBrush(Color.DarkGray), 0, 0, Width, Height);
            gBmp.DrawLine(new Pen(Color.Black), 6, 6, 6, bezierPlotsBitmap.Height - 6);
            gBmp.DrawLine(new Pen(Color.Black), 6, bezierPlotsBitmap.Height - 6, bezierPlotsBitmap.Width, bezierPlotsBitmap.Height - 6);

            if (ShowAllCheckbox.Checked)
            {
                for (int i = 0; i < 4; i++)
                    DrawLine(i, colorList[i]);
            }
            else
                DrawLine(ActiveIndex, colorList[ActiveIndex]);
            bezierCurves = GetBezierCurves();
        }

        public void InitializePlot()
        { 
            bezierControlPoints = new List<List<Point>>();
            for(int i = 0; i < 4; i++ )
            {
                List<Point> controlPoints = new List<Point>()
                {
                    new Point(6, bezierPlotsBitmap.Height - 6),
                    new Point(bezierPlotsBitmap.Width / 3, bezierPlotsBitmap.Height / 3 * 2),
                    new Point(bezierPlotsBitmap.Width / 3 * 2, bezierPlotsBitmap.Height/ 3),
                    new Point(bezierPlotsBitmap.Width - 6, 6)
                };
                bezierControlPoints.Add(controlPoints);
            }
            DrawPlot();
        }

        public CMYKForm()
        {
            colorList = new List<Color>()
            {
                Color.Cyan, Color.Magenta, Color.Yellow, Color.Black
            };
            ActiveIndex = 0;
            InitializeComponent();
            bezierPlotsBitmap = new Bitmap(BezierPlot.Width, BezierPlot.Height);
            BezierPlot.Image = bezierPlotsBitmap;
            InitializePlot();
        }

        private void ChangePictureButton_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "Image Files (*.jpg)|*.jpg|(*.png)|*.png|(*.gif)|*.gif|(*.jpeg)|*.jpeg|(*.bmp)|*.bmp";
            openFileDialog.CheckFileExists = true;
            openFileDialog.CheckPathExists = true;
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string filepath = openFileDialog.FileName;
                mainImage = Image.FromFile(filepath);
                CurrentImage.Image = mainImage;
                mainImageBitmap = new Bitmap(mainImage);
            }
        }



        private Color GetCmykFromPixel(int i, int j, Color color)
        {
            double c = 1.0 - (double)mainImageBitmap.GetPixel(i, j).R / 255.0;
            double m = 1.0 - (double)mainImageBitmap.GetPixel(i, j).G / 255.0;
            double y = 1.0 - (double)mainImageBitmap.GetPixel(i, j).B / 255.0;
            double k = Math.Min(Math.Min(c, m), y);
            c = c - k;
            m = m - k;
            y = y - k;
            
            double ck = BezierPlot.Height - bezierCurves[0].FirstOrDefault(p => p.X >= k * BezierPlot.Width).Y;
            ck = ck / BezierPlot.Height;
            c += ck;
            c = Math.Min(1, c);
            c = Math.Max(0, c);
            double mk = BezierPlot.Height - bezierCurves[1].FirstOrDefault(p => p.X >= k * BezierPlot.Width).Y;
            mk = mk / BezierPlot.Height;
            m += mk;
            m = Math.Min(1, m);
            m = Math.Max(0, m);
            double yk = BezierPlot.Height - bezierCurves[2].FirstOrDefault(p => p.X >= k * BezierPlot.Width).Y;
            yk = yk / BezierPlot.Height;
            y += yk;
            y = Math.Min(1, y);
            y = Math.Max(0, y);
            k = BezierPlot.Height - bezierCurves[3].FirstOrDefault(p => p.X >= k * BezierPlot.Width).Y;
            k = k / BezierPlot.Height;
            if(color == Color.Cyan)
                return Color.FromArgb(255 - (int)(c * 255), 255, 255);
            if (color == Color.Magenta)
                return Color.FromArgb(255, 255 - (int)(m * 255), 255);
            if (color == Color.Yellow)
                return Color.FromArgb(255, 255, 255 - (int)(y * 255));
            return Color.FromArgb(255 - (int) (k * 255),
                255 - (int)(k * 255),
                255 - (int)(k * 255));
        }

        private Bitmap GenerateBitmapFromRules(Color color) 
        {
            if (mainImageBitmap is null) throw new InvalidDataException();
            Bitmap result = new Bitmap(mainImageBitmap);
            for (int i = 0; i < result.Width; i++)
                for (int j = 0; j < result.Height; j++)
                {
                    var cmyk = GetCmykFromPixel(i, j, color);

                    result.SetPixel(i, j, cmyk);
                }
            return result;
        }

        private void ShowAllButton_Click(object sender, EventArgs e)
        {
            if (mainImage is null) return;
            CyanImage.Image = GenerateBitmapFromRules(Color.Cyan);
            MagentaImage.Image = GenerateBitmapFromRules(Color.Magenta);
            YellowImage.Image = GenerateBitmapFromRules(Color.Yellow);
            BlackAndWhiteImage.Image = GenerateBitmapFromRules(Color.Black);

        }

        private void BezierPlot_MouseDown(object sender, MouseEventArgs e)
        {
            for(int i = 0; i < bezierControlPoints[ActiveIndex].Count; i++)
            {
                var point = bezierControlPoints[ActiveIndex][i];
                if (e.Location.X > point.X - 6 && e.Location.X < point.X + 6 &&
                    e.Location.Y > point.Y - 6 && e.Location.Y < point.Y + 6)
                {
                    ActiveControlPoint = i;
                    return;
                }
            }
            ActiveControlPoint = -1;
        }

        private void BezierPlot_MouseUp(object sender, MouseEventArgs e)
        {
            ActiveControlPoint = -1;
        }

        private void Radio_CheckedChanged(object sender, EventArgs e)
        {
            RadioButton snd = sender as RadioButton;
            ActiveIndex = Int32.Parse((string)snd.Tag);

            DrawPlot();
        }

        private void ShowAllCheckbox_CheckedChanged(object sender, EventArgs e)
        {
            DrawPlot();
        }

        private void BezierPlot_MouseMove(object sender, MouseEventArgs e)
        {
            Point toSetUp = new Point(e.Location.X, e.Location.Y);
            if (toSetUp.X < 6) toSetUp.X = 6;
            if (toSetUp.X > bezierPlotsBitmap.Width) toSetUp.X = bezierPlotsBitmap.Width;
            if (toSetUp.Y < 6) toSetUp.Y = 6;
            if (toSetUp.Y > bezierPlotsBitmap.Height) toSetUp.Y = bezierPlotsBitmap.Height;

            if (ActiveControlPoint == -1) return;
            else if (ActiveControlPoint == 0 || 
                ActiveControlPoint == bezierControlPoints[ActiveIndex].Count - 1)
                bezierControlPoints[ActiveIndex][ActiveControlPoint] =
                   new Point(
                        bezierControlPoints[ActiveIndex][ActiveControlPoint].X,
                        toSetUp.Y
                    );
            else
                bezierControlPoints[ActiveIndex][ActiveControlPoint] = toSetUp;
            DrawPlot();
        }

        private void SaveCurveButton_Click(object sender, EventArgs e)
        {
            saveFileDialog.Filter = "Bezier Files (*.bez)|*.bez";
            saveFileDialog.RestoreDirectory = true;
            if (saveFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string filepath = saveFileDialog.FileName;

                XmlSerializer ser = new XmlSerializer(typeof(List<Point>));
                TextWriter writer = new StreamWriter(filepath);
                ser.Serialize(writer, bezierControlPoints[ActiveIndex]);
                writer.Close();
            }
        }

        private void LoadCurveButton_Click(object sender, EventArgs e)
        {
            openFileDialog.Filter = "Bezier Files (*.bez)|*.bez"; 
            openFileDialog.CheckFileExists = true;
            openFileDialog.CheckPathExists = true;
            if (openFileDialog.ShowDialog(this) == DialogResult.OK)
            {
                string filepath = openFileDialog.FileName;

                XmlSerializer ser = new XmlSerializer(typeof(List<Point>));
                using (Stream reader = new FileStream(filepath, FileMode.Open))
                {
                    var deserialized = ser.Deserialize(reader) as List<Point>;
                    if (deserialized is null) return;
                    bezierControlPoints[ActiveIndex] = deserialized;
                }
                DrawPlot();
            }
        }

        private void FullBackButton_Click(object sender, EventArgs e)
        {
            bezierControlPoints = new List<List<Point>>()
            {
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 156,Y = 377 },
                    new Point() { X = 305,Y = 386 },
                    new Point() { X = 474,Y = 381 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 156,Y = 377 },
                    new Point() { X = 305,Y = 386 },
                    new Point() { X = 474,Y = 381 }
                },                
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 156,Y = 377 },
                    new Point() { X = 305,Y = 386 },
                    new Point() { X = 474,Y = 381 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 160,Y = 260 },
                    new Point() { X = 320,Y = 130 },
                    new Point() { X = 474,Y = 6 }
                }
            };
            DrawPlot();
        }

        private void NoBackingButton_Click(object sender, EventArgs e)
        {
            bezierControlPoints = new List<List<Point>>()
            {
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 28,Y = 371 },
                    new Point() { X = 463,Y = 12 },
                    new Point() { X = 474,Y = 6 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 295,Y = 253 },
                    new Point() { X = 367,Y = 94 },
                    new Point() { X = 474,Y = 6 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 283,Y = 256 },
                    new Point() { X = 348,Y = 146 },
                    new Point() { X = 474,Y = 6 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 160,Y = 386 },
                    new Point() { X = 320,Y = 386 },
                    new Point() { X = 474,Y = 386 }
                }
            };
            DrawPlot();
        }

        private void UCRButton_Click(object sender, EventArgs e)
        {
            bezierControlPoints = new List<List<Point>>()
            { 
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 436,Y = 53 },
                    new Point() { X = 336,Y = 84 },
                    new Point() { X = 474,Y = 6 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 357,Y = 208 },
                    new Point() { X = 313,Y = 110 },
                    new Point() { X = 474,Y = 82 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 374,Y = 213 },
                    new Point() { X = 308,Y = 115 },
                    new Point() { X = 474,Y = 127 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 480,Y = 392 },
                    new Point() { X = 435,Y = 392 },
                    new Point() { X = 474,Y = 8 }
                }
            };
            DrawPlot();
        }

        private void GCRButton_Click(object sender, EventArgs e)
        {
            bezierControlPoints = new List<List<Point>>()
            {
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 436,Y = 33 },
                    new Point() { X = 465,Y = 105 },
                    new Point() { X = 474,Y = 99 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 316,Y = 278 },
                    new Point() { X = 294,Y = 145 },
                    new Point() { X = 474,Y = 129 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 247,Y = 318 },
                    new Point() { X = 304,Y = 151 },
                    new Point() { X = 474,Y = 127 }
                },
                new List<Point>()
                {
                    new Point() { X = 6,Y = 386 },
                    new Point() { X = 252,Y = 392 },
                    new Point() { X = 412,Y = 392 },
                    new Point() { X = 474,Y = 6 }
                }
            };
            DrawPlot();
        }

        private void BlackAdnWhiteButton_Click(object sender, EventArgs e)
        {
            if (mainImageBitmap is null) return;
            using (Form form = new Form())
            {
                Bitmap img = GenerateBitmapFromRules(Color.Black);

                form.StartPosition = FormStartPosition.CenterScreen;
                form.Size = img.Size;

                PictureBox pb = new PictureBox();
                pb.Dock = DockStyle.Fill;
                pb.Image = img;

                form.Controls.Add(pb);
                form.ShowDialog();
            }
        }

        private void SavePicturesButton_Click(object sender, EventArgs e)
        {
            if (mainImageBitmap is null || CyanImage.Image is null) return;
            if (folderBrowserDialog1.ShowDialog() == DialogResult.OK)
            {
                string path = folderBrowserDialog1.SelectedPath;
                CyanImage.Image.Save(path + "\\cyan.png");
                MagentaImage.Image.Save(path + "\\magenta.png");
                YellowImage.Image.Save(path + "\\yellow.png");
                BlackAndWhiteImage.Image.Save(path + "\\black.png");
            }
        }
    }
}