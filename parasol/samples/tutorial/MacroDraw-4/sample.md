// sample.md
// sample script for MacroDraw-4


float pi = 4 * atan(1.0);

macro Hello
{
    Canvas* canvas = getCanvas();

    int n = 20;
    for (int i = 0; i <= n; ++i) {
	float x = 80.0 * i / n + 10;

	float y0 = -20 * cos(3 * pi * i / n) + 50;
	float r0 = -0.5 * sin(3 * pi * i / n) + 2;
	canvas->drawCircle(x, y0, r0);

	if (i == n / 2) {
	    continue;
	}

	float y1 = 20 * cos(3 * pi * i / n) + 50;
	float r1 = 0.5 * sin(3 * pi * i / n) + 2;
	canvas->drawRect(x - r1 / 2, y1 - r1 / 2, x + r1 / 2, y1 + r1 / 2);

	float w = abs(y1 - y0) - (r0 + r1) - 4;
	canvas->drawLine(x, 50 - w / 2, x, 50 + w / 2);
    }

    canvas->drawText(5, 15, "Hello, MacroDraw!");
}
