float LinearizeDepth(float z)
{
float n = 1; // camera z near
float f = 10000.; // camera z far
return (n * z) / ( f - z * (f - n) );
}

uniform sampler2D depthImg;

void main()
{
float depth = texture2D(depthImg, gl_TexCoord[0].xy).r;
depth = LinearizeDepth(depth) * 1.;//depthValModifier;

gl_FragColor = vec4(depth, depth, depth, 1.0);
}