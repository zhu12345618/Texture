
attribute vec3 vert;
attribute vec2 vertTexCoord;

varying vec2 textureOut;

void main() {
    // Pass the tex coord straight through to the fragment shader
    textureOut = vertTexCoord;
    
    gl_Position =  vec4(vert, 1);
}
