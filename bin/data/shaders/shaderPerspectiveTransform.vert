
uniform sampler2DRect depthTex; // depth
uniform vec4 u_camParams;
uniform mat4 u_viewMatrix;
uniform mat4 u_clippedProjMatrix;


vec3 getWorldCoordinate(float x, float y, float z, vec4 camParams){
    vec3 world;
    world.z = z;
    world.x = (x - camParams.x) * z / camParams.z;
    world.y = -(y - camParams.y) * z / camParams.w;
    return world;
}
float nfClip(float low1, float high1, float value){
    float low2 = 1.0;
    float high2 = 0.0;
    float d = clamp(low2 + (value - low1) * (high2 - low2) / (high1 - low1), 0.0, 1.0);
    if (d == 1.0) {
        d = 0.0;
    }
    return d;
}

void main(void)
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    
    vec4 pos = gl_Vertex;
    float depth= texture2DRect(depthTex, gl_TexCoord[0].xy).r/1000.;
    if(depth > 3.5) depth = 0.;
    vec3 worldPos = getWorldCoordinate(pos.x, pos.y, depth, u_camParams);
    vec4 screenPos = u_clippedProjMatrix * u_viewMatrix * vec4(worldPos, 1.);

    float dist = distance(vec4(0.,0.,0.,0.), screenPos)*1000.;
    screenPos.y *= -1.;
    gl_Position = screenPos;
    gl_FrontColor = vec4(vec3(nfClip(10., 200., dist)), 1.);
    gl_PointSize  = .01;
}