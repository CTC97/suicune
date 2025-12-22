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
    vec4 glow = vec4(0.0);

    // Sample 4 neighbors for a glow effect
    float offset = 1.0 / resolution.x; // assuming square texture

    glow += texture(texture0, fragTexCoord + vec2(offset, 0.0));
    glow += texture(texture0, fragTexCoord + vec2(-offset, 0.0));
    glow += texture(texture0, fragTexCoord + vec2(0.0, offset));
    glow += texture(texture0, fragTexCoord + vec2(0.0, -offset));

    glow *= 0.12; // average neighbors

    // Pulse intensity
    float pulse = 0.2 + 0.8 * sin(time * 6.2831);

    // Combine center with a yellowish glow around
    vec3 color = center.rgb + glow.rgb * pulse * 0.5;

    finalColor = vec4(color, center.a) * fragColor;
}
