import matplotlib.pyplot as plt
import matplotlib


def plot_data(path: str):
    with open(path) as file:
        array = [float(row.strip()) for row in file]

    plt.plot(array)
    plt.grid()
    plt.ylim([0.0, max(array)])
    plt.xlim([0.0, len(array)])
    plt.xlabel("Эпоха")
    plt.ylabel("Ошибка")
    plt.title("График изменения ошибки")
    plt.savefig('error.png')


if __name__ == '__main__':
    plot_data("data.txt")
