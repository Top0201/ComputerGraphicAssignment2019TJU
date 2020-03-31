#version 410 core

layout( vertices = 25) out;

uniform float uOuterLR, uOuterUD, uInnerC, uInnerR;

void main(){
	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	// set tessellation levels
	gl_TessLevelOuter[0] = uOuterLR;//left
    gl_TessLevelOuter[1] = uOuterUD;//down
    gl_TessLevelOuter[2] = uOuterLR;//right
    gl_TessLevelOuter[3] = uOuterUD;//upper
    gl_TessLevelInner[0] = uInnerC;//column
    gl_TessLevelInner[1] = uInnerR;//row
}
