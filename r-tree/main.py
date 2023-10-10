import matplotlib.pyplot as plt
colors_rgb = [(0, 0, 255), (0, 255, 0), (255, 0, 0), (0, 255, 255), (255, 0, 255)]
colors_normalized = [(r / 255, g / 255, b / 255) for r, g, b in colors_rgb]

with open(r"C:\Users\Hillary\Downloads\untitled20\prueba.txt", "r") as file:

    current_level = None

    with open(r"C:\Users\Hillary\Downloads\untitled20\prueba 2.txt", "r") as niveles:
        for line in file:

            numbers = line.strip().split()
            level = int(niveles.readline().strip())
            mbr = [(int(numbers[j]), int(numbers[j + 1])) for j in range(0, len(numbers), 2)]
            x_values, y_values = zip(*mbr)
            x_values += (x_values[0],)
            y_values += (y_values[0],)
            if level != current_level:
                current_level = level
                color_normalized = colors_normalized[current_level % len(colors_normalized)]
            plt.plot(x_values, y_values, label=f'MBR (level {level})', color=color_normalized)

plt.xlabel('Coordenada X')
plt.ylabel('Coordenada Y')
plt.grid(True)
plt.legend()
plt.show()
