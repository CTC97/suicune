#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution; // texture size in pixels

// Simple hash-based noise (no extra uniforms)
float hash(vec2 p)
{
    // cheap + stable
    return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float smoothNoise(vec2 p)
{
    vec2 i = floor(p);
    vec2 f = fract(p);

    float a = hash(i);
    float b = hash(i + vec2(1.0, 0.0));
    float c = hash(i + vec2(0.0, 1.0));
    float d = hash(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(mix(a, b, u.x), mix(c, d, u.x), u.y);
}

void main()
{
    vec4 base = texture(texture0, fragTexCoord);

    // Pixel coords in "screen/texture pixels"
    vec2 uv = fragTexCoord;
    vec2 px = uv * resolution;

    // --- Snow layers (3 parallax bands) ---
    float snow = 0.0;

    // Tuning knobs (keep subtle so it reads as "snow" not "TV static")
    float wind1 = 12.0;
    float wind2 = 24.0;
    float wind3 = 40.0;

    // Layer 1: big/slow flakes
    {
        vec2 p = px * 0.035;
        p.y += time * 24.0;
        p.x += sin((p.y + time) * 0.35) * wind1 * 0.02;

        float n = smoothNoise(p);
        // sparse bright flakes
        float flakes = smoothstep(0.92, 0.985, n);
        // little twinkle so it doesn't feel frozen
        flakes *= 0.65 + 0.35 * sin(time * 3.0 + n * 12.0);
        snow += flakes * 0.55;
    }

    // Layer 2: medium flakes
    {
        vec2 p = px * 0.07;
        p.y += time * 46.0;
        p.x += sin((p.y + time) * 0.55) * wind2 * 0.02;

        float n = smoothNoise(p + 13.37);
        float flakes = smoothstep(0.935, 0.992, n);
        flakes *= 0.7 + 0.3 * sin(time * 4.2 + n * 10.0);
        snow += flakes * 0.45;
    }

    // Layer 3: tiny/fast specks
    {
        vec2 p = px * 0.12;
        p.y += time * 72.0;
        p.x += sin((p.y + time) * 0.8) * wind3 * 0.02;

        float n = smoothNoise(p + 71.0);
        float flakes = smoothstep(0.955, 0.997, n);
        snow += flakes * 0.35;
    }

    // Slight vertical fade: more flakes near top
    float fade = smoothstep(1.0, 0.0, uv.y);
    snow *= mix(0.6, 1.0, fade);

    // Optional: tiny motion blur (a faint streak) by sampling "upwind"
    // Keep it subtle and resolution-aware
    vec2 streak = vec2(0.6, -1.0) / resolution * 2.0;
    float streakN = smoothNoise((px * 0.09) + vec2(time * 18.0, time * 50.0));
    float streakMask = smoothstep(0.965, 0.995, streakN) * 0.15;
    snow += streakMask;

    // Snow color (slightly cool)
    vec3 snowColor = vec3(0.92, 0.96, 1.0);

    // Composite: add snow on top, preserve alpha
    vec3 color = base.rgb + snowColor * snow;

    finalColor = vec4(color, base.a) * fragColor;
}
