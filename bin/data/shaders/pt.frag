uniform sampler2DRect depthTex; // depth
uniform vec4 u_camParams;
uniform mat4 u_newViewMatrix;

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

void main()
{
    vec4 col = texture2DRect(depthTex, gl_TexCoord[0].xy);
    //float value = col.r;
    float value = nfClip( 100., 2000., col.r);
    // dept to world
    vec3 world = getWorldCoordinate(gl_TexCoord[0].x, gl_TexCoord[0].y, col.r, u_camParams);
    
    //                  vec3 view = world*u_newViewMatrix;
    //                  float fogz = mul(UNITY_MATRIX_MV, v.vertex).z;
    
    
    //                  gl_FragColor = texture2DRect(tex, gl_TexCoord[0].xy);
    
    gl_FragColor = vec4(vec3(value), 1.0);
    
}