//void main(void)
//{
//    gl_Position = ftransform();
//    gl_TexCoord[0] = gl_MultiTexCoord0;
//}
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

void main(void)
{
	   vec4 col = texture2DRect(depthTex, gl_TexCoord[0].xy);
    //float value = col.r;
    float value = col.r;
    // dept to world
    vec3 world = getWorldCoordinate(gl_TexCoord[0].x, gl_TexCoord[0].y, col.r, u_camParams);


    gl_Position = ftransform();
    gl_TexCoord[0] = gl_MultiTexCoord0;
}