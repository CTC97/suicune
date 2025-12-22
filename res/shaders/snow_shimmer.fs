#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform float time;
uniform vec2 resolution;

float hash21(vec2 p)
{
    // Cheap, decent hash (no extra textures)
    p = fract(p * vec2(123.34, 345.45));
    p += dot(p, p + 34.345);
    return fract(p.x * p.y);
}

float luma(vec3 c)
{
    return dot(c, vec3(0.299, 0.587, 0.114));
}

void main()
{
    vec2 uv = fragTexCoord;
    vec4 src = texture(texture0, uv);

    // Identify "snow-ish": bright and not very saturated
    float lum = luma(src.rgb);
    float sat = max(src.r, max(src.g, src.b)) - min(src.r, min(src.g, src.b));

    float snowMask = smoothstep(0.60, 0.92, lum) * (1.0 - smoothstep(0.08, 0.22, sat));
    // snowMask ~ 0 on dark/colored pixels, ~1 on bright neutral pixels

    // Pixel space
    vec2 pix = uv * resolution;

    // Drift: move the sparkle field slowly (diagonal wind)
    vec2 drift = vec2(time * 9.0, time * 4.5);

    // Two layers of sparkles at different scales for richness
    float sparkle = 0.0;

    // ---- Layer A (fine glitter) ----
    {
        float cellSize = 8.0; // grid in pixels
        vec2 g = floor((pix + drift) / cellSize);
        vec2 f = fract((pix + drift) / cellSize);

        float rnd = hash21(g);
        // Each cell has a "spark point"
        vec2 center = vec2(hash21(g + 1.7), hash21(g + 9.2));

        float d = length(f - center);

        // Twinkle: each cell gets its own phase
        float tw = 0.5 + 0.5 * sin(time * 6.0 + rnd * 6.2831);

        // Small, sharp sparkle blob
        float star = smoothstep(0.18, 0.00, d) * smoothstep(0.25, 0.95, tw);

        sparkle += star * 0.65;
    }

    // ---- Layer B (rare bright glints) ----
    {
        float cellSize = 18.0;
        vec2 g = floor((pix + drift * 0.6 + 50.0) / cellSize);
        vec2 f = fract((pix + drift * 0.6 + 50.0) / cellSize);

        float rnd = hash21(g + 3.3);
        vec2 center = vec2(hash21(g + 4.1), hash21(g + 7.9));

        float d = length(f - center);

        // Rare sparkle gate: only some cells can glint
        float gate = step(0.82, rnd);

        float tw = 0.5 + 0.5 * sin(time * 4.0 + rnd * 8.0);
        float glint = smoothstep(0.12, 0.00, d) * smoothstep(0.55, 0.98, tw) * gate;

        sparkle += glint * 1.25;
    }

    // Gentle scanline-y modulation so it feels like light catching uneven snow
    float micro = 0.9 + 0.1 * sin((pix.y + time * 30.0) * 0.15);

    // Apply sparkle only on snowy pixels
    float intensity = sparkle * micro * snowMask;

    // Add a cool-white tint (keeps it "snow" not "glitter bomb")
    vec3 shimmerColor = vec3(0.75, 0.90, 1.00) * intensity;

    // Preserve alpha; keep edges crisp (no neighbor sampling)
    vec3 outRgb = src.rgb + shimmerColor;

    finalColor = vec4(outRgb, src.a) * fragColor;
}
