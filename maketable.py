import re
import os
import sys
import glob
import pandas as pd
import matplotlib.pyplot as plt

def main():
    input_filename = "data.txt"
    if len(sys.argv) > 1:
        input_filename = sys.argv
        
    if not os.path.exists(input_filename):
        print(f"Ошибка! Не найден файл с данными: {input_filename}")
        return

    # Очищаем папку от всех старых картинок перед стартом
    old_plots = glob.glob("График_*.png") + glob.glob("_Общее_*.png") + glob.glob("_Сравнение_*.png")
    for plot_path in old_plots:
        try:
            os.remove(plot_path)
        except Exception:
            pass

    print(f"Читаю данные из файла: {input_filename}...")
    with open(input_filename, "r", encoding="utf-8") as f:
        raw_data = f.read()

    # --- ПАРСИНГ ДАННЫХ ---
    rows = []
    current_n = None
    current_row = {}
    unique_algos = set()

    n_pattern = re.compile(r"(\d+)\s+elements:")
    algo_pattern = re.compile(r"\s+(.+) - build:\s*(\d+),\s*query:\s*(\d+)(?:,\s*update:\s*(\d+))?")

    for line in raw_data.strip().split("\n"):
        n_match = n_pattern.match(line)
        if n_match:
            if current_row: rows.append(current_row)
            current_n = int(n_match.group(1))
            current_row = {"n": current_n}
            continue

        algo_match = algo_pattern.match(line)
        if algo_match and current_n is not None:
            algo_name = algo_match.group(1).strip()
            unique_algos.add(algo_name)
            current_row[f"{algo_name} (Build)"] = int(algo_match.group(2))
            current_row[f"{algo_name} (Query)"] = int(algo_match.group(3))
            if algo_match.group(4) is not None:
                current_row[f"{algo_name} (Update)"] = int(algo_match.group(4))

    if current_row: rows.append(current_row)

    if not rows:
        print("Ошибка парсинга данных.")
        return

    df = pd.DataFrame(rows)
    df = df.sort_values(by="n").reset_index(drop=True)

    operation_styles = {
        "Build":  {"color": "#1f77b4", "marker": "o", "label": "Построение (Build)"},
        "Query":  {"color": "#ff7f0e", "marker": "s", "label": "Запрос (Query)"},
        "Update": {"color": "#2ca02c", "marker": "^", "label": "Обновление (Update)"}
    }

    # --- 1. ПОСТРОЕНИЕ ОДИНОЧНЫХ ЛИНЕЙНЫХ ГРАФИКОВ ---
    print("Генерирую линейные графики для каждого алгоритма...")
    for algo in sorted(list(unique_algos)):
        plt.figure(figsize=(10, 6))
        has_plots = False
        
        for op_name, style in operation_styles.items():
            col_name = f"{algo} ({op_name})"
            
            if col_name in df.columns and not df[col_name].isna().all():
                plt.plot(
                    df["n"], 
                    df[col_name], 
                    marker=style["marker"], 
                    color=style["color"], 
                    label=style["label"], 
                    linewidth=2.5, 
                    markersize=7
                )
                has_plots = True
                
        if has_plots:
            plt.title(f"Сложность алгоритма: {algo}", fontsize=14, fontweight='bold', pad=15)
            plt.xlabel("Количество элементов (n)", fontsize=11, labelpad=8)
            plt.ylabel("Количество операций", fontsize=11, labelpad=8)
            
            plt.xscale("linear")
            plt.yscale("linear")
            plt.grid(True, linestyle="--", alpha=0.5)
            
            plt.xticks(df["n"], labels=[str(x) for x in df["n"]])
            plt.xticks(rotation=15, fontsize=9)
            plt.yticks(fontsize=9)
            
            plt.gca().yaxis.get_major_formatter().set_scientific(False)
            plt.legend(loc="best", fontsize=10)
            plt.tight_layout()
            
            safe_filename = algo.replace("/", "_").replace("^", "2").replace("(", "").replace(")", "")
            plt.savefig(f"График_{safe_filename}.png", dpi=150)
            plt.close()

    # Палитра для общих графиков сравнения
    colors_palette = ['#1f77b4', '#ff7f0e', '#2ca02c', '#d62728', '#9467bd', '#8c564b', '#e377c2', '#7f7f7f']
    markers_palette = ['o', 's', '^', 'D', 'v', 'p', '*', 'h']
    
    # Чтобы цвета одних и тех же алгоритмов совпадали на обоих графиках, зафиксируем карту цветов
    algo_list = sorted(list(unique_algos))
    algo_style_map = {}
    for i, algo in enumerate(algo_list):
        algo_style_map[algo] = {
            "color": colors_palette[i % len(colors_palette)],
            "marker": markers_palette[i % len(markers_palette)]
        }

    # --- 2. ПОСТРОЕНИЕ ДВУХПАНЕЛЬНОГО ОБЩЕГО ГРАФИКА (ЧЕСТНЫЙ ЛИНЕЙНЫЙ МАСШТАБ) ---
    print("Генерирую двухпанельный общий сравнительный график (Build)...")
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(16, 7))
    fig.suptitle("Сравнительный анализ времени построения (Build) в линейном масштабе", fontsize=16, fontweight='bold', y=0.98)

    # Алгоритмы-тяжеловесы, которые нужно исключить на второй панели
    heavy_algos = ["RMQ1D (Naive O(n^2) table)", "RSQ2D (2D Prefix Sum)"]

    for algo in algo_list:
        col_name = f"{algo} (Build)"
        if col_name in df.columns:
            style = algo_style_map[algo]
            
            # Рисуем ВСЕ алгоритмы на левом графике
            ax1.plot(
                df["n"], df[col_name], 
                marker=style["marker"], color=style["color"], 
                label=algo, linewidth=2.5, markersize=6
            )
            
            # Рисуем алгоритмы без "тяжеловесов" на правом графике
            if algo not in heavy_algos:
                ax2.plot(
                    df["n"], df[col_name], 
                    marker=style["marker"], color=style["color"], 
                    label=algo, linewidth=2.5, markersize=6
                )

    # Настройка ЛЕВОГО графика (Абсолютно все алгоритмы)
    ax1.set_title("Все алгоритмы целиком (Виден масштаб O(n²))", fontsize=12, pad=12, fontweight='bold')
    ax1.set_xlabel("Количество элементов (n)", fontsize=10, labelpad=5)
    ax1.set_ylabel("Количество операций", fontsize=10)
    ax1.grid(True, linestyle="--", alpha=0.5)
    ax1.set_xticks(df["n"])
    ax1.set_xticklabels([str(x) for x in df["n"]], rotation=20, fontsize=8)
    ax1.yaxis.get_major_formatter().set_scientific(False)
    ax1.legend(loc="upper left", fontsize=8, frameon=True)

    # Настройка ПРАВОГО графика (Исключены тяжелые структуры)
    ax2.set_title("Без Naive O(n²) и 2D Prefix Sum (Виден масштаб быстрых структур)", fontsize=12, pad=12, fontweight='bold')
    ax2.set_xlabel("Количество элементов (n)", fontsize=10, labelpad=5)
    ax2.set_ylabel("Количество операций", fontsize=10)
    ax2.grid(True, linestyle="--", alpha=0.5)
    ax2.set_xticks(df["n"])
    ax2.set_xticklabels([str(x) for x in df["n"]], rotation=20, fontsize=8)
    ax2.yaxis.get_major_formatter().set_scientific(False)
    ax2.legend(loc="upper left", fontsize=8, frameon=True)

    plt.tight_layout()
    plt.savefig("_Общее_Сравнение_Две_Панели.png", dpi=150)
    plt.close()

    print("\n[УСПЕХ] Все одиночные линейные графики обновлены.")
    print(" -> Создана итоговая двухпанельная картинка: _Общее_Сравнение_Две_Панели.png")

if __name__ == "__main__":
    main()
