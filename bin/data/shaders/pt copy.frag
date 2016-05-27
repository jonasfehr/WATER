uniform sampler2DRect depthTex; // depth



//void main(void)
//{
//    gl_Position = ftransform();
//    gl_TexCoord[0] = gl_MultiTexCoord0;
//}
uniform sampler2DRect depthTex; // depth
uniform vec4 u_camParams;
uniform mat4 u_viewMatrix;
uniform mat4 u_clippedProjMatrix;


vec3 getWorldCoordinate(vec3 pos, vec4 camParams){
    vec3 world;
    world.z = pos.z;
    world.x = (pos.x - camParams.x) * pos.z / camParams.z;
    world.y = -(pos.y - camParams.y) * pos.z / camParams.w;
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

void main()
{

    float value = nfClip( 100., 2000., col.r);


    vec4 pos = vec3(gl_TexCoord[0].xy, texture2DRect(depthTex, gl_TexCoord[0].xy).r);
    // vec3 worldPos = getWorldCoordinate(pos, u_camParams);
 //    vec2 screenPos = worldPosition * u_viewMatrix * u_clippedProjMatrix;
 //    vec2 screenPosNorm = (screenPos + 1)/2;
 //    float dist = worldPos.z;
 //    gl_Position = vec3(screenPosNorm, dist);
    
    gl_FragColor = vec4(vec3(value), 1.0);
    
}