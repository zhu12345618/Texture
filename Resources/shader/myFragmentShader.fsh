
uniform sampler2D tex;

varying vec2 textureOut;

void main() {
    gl_FragColor = texture2D(tex, textureOut);
}
