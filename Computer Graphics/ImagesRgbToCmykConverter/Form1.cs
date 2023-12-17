using System.Net.Http.Headers;
using System.Windows.Forms;
using System.Xml.Serialization;

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

        public int ActiveIndex { get; set; }

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

        private Bitmap GenerateBitmapFromRules(Color color) 
        {
            if (mainImageBitmap is null) throw new InvalidDataException();
            Bitmap result = new Bitmap(mainImageBitmap);
            for (int i = 0; i < result.Width; i++)
                for (int j = 0; j < result.Height; j++)
                    result.SetPixel(i, j, color);
            return result;
        }

        private void ShowAllButton_Click(object sender, EventArgs e)
        {
            if (mainImage is null) return;
            CyanImage.Image = GenerateBitmapFromRules(Color.RebeccaPurple);
            MagentaImage.Image = GenerateBitmapFromRules(Color.BlueViolet);
            YellowImage.Image = GenerateBitmapFromRules(Color.GreenYellow);
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
    }
}