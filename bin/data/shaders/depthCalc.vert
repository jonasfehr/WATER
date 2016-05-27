uniform vec3 eye_position;

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
//     vec3 eye_position;
//     eye_position = vec3(0., 0., 0.);
     vec4 eyePos = gl_ModelViewMatrix * gl_Vertex;
     float dist = nfClip(1., 2., distance(vec4(eye_position,1.), gl_Position));
//     float dist = nfClip(1., 2., distance(eyePos, gl_Position));

     gl_Position = ftransform();
//     gl_FrontColor = vec4(vec3(dist), 1.);//gl_Color;
     gl_FrontColor = vec4(1., 0., dist, 1.);
//         gl_TexCoord[0] = gl_MultiTexCoord0;

 }


//void main(void)
//{
//    
////    vec4 eyePos = gl_ModelViewMatrix * gl_Vertex;
//
//    
//    gl_Position = ftransform();
//    gl_FrontColor = vec4(1., 0., 0., 1,);//gl_Color;
////    gl_TexCoord[0] = gl_MultiTexCoord0;
//}