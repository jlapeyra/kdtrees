        def hsv_to_rgb(h, s, v):
            r, g, b = colorsys.hsv_to_rgb(h, s, v)
            return (int(r * 255), int(g * 255), int(b * 255))

        def get_color(idx, total):
            h = idx / total
            s = 0.7
            v = 0.9
            r, g, b = colorsys.hsv_to_rgb(h, s, v)
            return (r, g, b)