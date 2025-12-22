#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution; // texture size in pixels

void main()
{
    vec4 center = texture(texture0, fragTexCoord);

    // Set the sprite color to red
    vec3 redColor = vec3(1.0, 0.0, 0.0);

    finalColor = vec4(redColor, center.a) * fragColor;
}