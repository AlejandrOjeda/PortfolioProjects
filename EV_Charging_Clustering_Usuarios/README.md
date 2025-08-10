# Patrones de Carga de Vehículos Eléctricos — Clustering de Usuarios

Este proyecto analiza patrones de carga de vehículos eléctricos (VE) mediante clustering (aprendizaje no supervisado) para identificar segmentos de comportamiento según:

- Ubicación: Home, Work, Other/Public
- Consumo medio (kWh)
- Duración y horario de carga

## Contenido
- `EV_Charging_Clustering_Usuarios_ES.ipynb`: notebook principal con todo el análisis.
- `figures/`: carpeta para guardar gráficos exportados.
- `requirements.txt`: librerías necesarias para reproducir el proyecto.
- `.gitignore`: exclusiones recomendadas para proyectos Python/Jupyter.

## Metodología
1. Importación y exploración inicial del dataset.
2. Ingeniería de variables relevantes (duración, hora de inicio, medias de consumo, etc.).
3. Escalado de variables.
4. Clustering con K-Means.
5. Visualizaciones e interpretación de resultados por ubicación.

## Resultados (resumen)
- Home: sesiones largas y mayor consumo, sobre todo en horario nocturno.
- Work: recargas más cortas y regulares durante la jornada laboral.
- Other/Public: sesiones puntuales y consumo variable.

## Próximos pasos
- Validar el número óptimo de clusters (método del codo y silhouette).
- Añadir variables como día de la semana o condiciones meteorológicas.
- Evaluar la estabilidad de los clusters en el tiempo.

## Cómo reproducir
```bash
python -m venv .venv
source .venv/bin/activate  # En Windows: .venv\Scripts\activate
pip install -r requirements.txt
jupyter lab
```

## Autor
© 2025 Tu Nombre. Licencia MIT.
