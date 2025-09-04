
#version 330 core

in vec3 colorFrag;
out vec4 color;

//const float Epsilon = 1e-10;
//const float hue = 0.2;
//const float saturation = 1;
//const float value = 0.5;
//
//vec3 RGBtoHSV(in vec3 RGB)
//{
//    vec4  P   = (RGB.g < RGB.b) ? vec4(RGB.bg, -1.0, 2.0/3.0) : vec4(RGB.gb, 0.0, -1.0/3.0);
//    vec4  Q   = (RGB.r < P.x) ? vec4(P.xyw, RGB.r) : vec4(RGB.r, P.yzx);
//    float C   = Q.x - min(Q.w, Q.y);
//    float H   = abs((Q.w - Q.y) / (6.0 * C + Epsilon) + Q.z);
//    vec3  HCV = vec3(H, C, Q.x);
//    float S   = HCV.y / (HCV.z + Epsilon);
//    return vec3(HCV.x, S, HCV.z);
//}



//vec3 HSVtoRGB(in vec3 HSV)
//{
//    float H   = HSV.x;
//    float R   = abs(H * 6.0 - 3.0) - 1.0;
//    float G   = 2.0 - abs(H * 6.0 - 2.0);
//    float B   = 2.0 - abs(H * 6.0 - 4.0);
//    vec3  RGB = clamp( vec3(R,G,B), 0.0, 1.0 );
//    return ((RGB - 1.0) * HSV.y + 1.0) * HSV.z;
//}


void main()
{
//    color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
//    vec3 col_hsv = RGBtoHSV(colorFrag);
//    col_hsv.z *= (value*2.0);
//    col_hsv.y *= (saturation* 2.0);
//    col_hsv.x *= (hue * 2.0);
//    vec3 col_rgb = HSVtoRGB(col_hsv.rgb);
    color = vec4(colorFrag.rgb, 1.0f);
}

