#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

uniform vec2 res;
uniform float time;

vec3 palette(float t)
{
	vec3 a = vec3(0.5, 0.5, 0.5);
	vec3 b = vec3(0.5, 0.5, 0.5);
	vec3 c = vec3(1, 1, 1);
	vec3 d = vec3(0.263, 0.416, 0.557);
	return a + b * cos(6.28318 * (c * t + d));
}

void main()
{
	vec2 uv = (gl_FragCoord.xy * 2.f - res.xy) / res.y;
	vec2 uv0 = uv;

	vec3 finalColor = vec3(0.0);

	for (float i = 0; i < 1; i++)
	{
		uv = fract(uv * 1.5f) - 0.5f;

		float d = length(uv) * exp(-length(uv0));

		vec3 col = palette(length(uv0) + time*.4 + i*.4);

		d = sin(d * 8 + time) / 8;
		d = abs(d);
		d = pow(0.01f / d, 1.2);

		finalColor += col * d;
	}
	

	color = vec4(finalColor, 0.5f);
};