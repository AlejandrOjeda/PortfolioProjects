import os # Leer variables de entorno (tokens,url etc)
import pandas as pd # Manipulación de dataframes
from influxdb_client import InfluxDBClient, Point # Cliente de InfluxDB
from influxdb_client.client.write_api import SYNCHRONOUS#
import joblib # Cargar el modelo guardado de ML

#Previamente he creado un archivo .env con las variables de entorno necesarias, esto es importante porque
# no queremos dejar tokens y urls en el código

#Con la funcion getenv de os, leemos las variables de entorno

INFLUX_URL = os.getenv("INFLUX_URL") 
INFLUX_TOKEN = os.getenv("INFLUX_TOKEN")
INFLUX_ORG = os.getenv("INFLUX_ORG")
INFLUX_BUCKET = os.getenv("INFLUX_BUCKET")

MODEL_PATH = os.getenv("MODEL_PATH", "pipeline_RandomForest.pkl") # Lo que hara es buscar una variable de entorno 
#llamada MODEL_PATH, y si no la encuentra, usara el valor por defecto "Forecasting_randomForest.pkl"

MEASUREMENT = os.getenv("INFLUX_MEASUREMENT", "ambiente") # lo mismo que antes, pero para la medición de InfluxDB

OUT_FIELD_1H = "pm25_pred_1h"
OUT_FIELD_3H = "pm25_pred_3h"



client = InfluxDBClient(url=INFLUX_URL, token=INFLUX_TOKEN, org=INFLUX_ORG)# El cliente de InfluxDB que usaremos para leer y escribir
query_api = client.query_api() # API para hacer consultas
write_api = client.write_api(write_options=SYNCHRONOUS) # API para escribir datos

# Consulta Flux para obtener los datos de las últimas 6 horas de la medición
#  "ambiente" y el campo "pm2_5"

flux = f'''
from(bucket: "{INFLUX_BUCKET}")
  |> range(start: -6h)
  |> filter(fn: (r) => r._measurement == "ambiente")
  |> filter(fn: (r) => r._field == "pm2_5")
  |> sort(columns: ["_time"])

'''
#Creo el dataframe a partir de la consulta
df = query_api.query_data_frame(flux)



if isinstance(df, list):
    df = pd.concat(df)

# paso la columna _time a tipo datetime y la pongo como índice
df["_time"] = pd.to_datetime(df["_time"])
df = df.set_index("_time")


# nos quedamos solo con la columna _value
df = df[["_value"]]

#la renombro al nombre queutilizado para entrenar el modelo pm2.5
df = df.rename(columns={"_value": "pm2_5"})

# Ordenamos por tiempo (siempre)
df = df.sort_index()

# Resamplea a 5min y rellena huecos. Seria necesario si hay mucho huevo hacer un resample hasta 15 mins solo ? 
df = df.resample("5min").mean()
df = df.ffill()
# Convertimos a Serie, que es lo que espera skforecast
y = df["pm2_5"]

#La longitud de este debe ser minimo de 12 para que el modelo pueda hacer la predicción

if len(y) < 12:
    raise ValueError(f"No hay suficientes datos para predecir: {len(y)} puntos (mínimo 12)")
# cargo el modelo guardado (ForecasterRecursive ya entrenado)
forecaster = joblib.load(MODEL_PATH)

# ventana de entrada: últimos 12 valores (lags 1..12)
last_window = y.iloc[-12:].reset_index(drop=True)

# hago una sola predicción hasta 3h (36 pasos) y saco 1h y 3h
pred = forecaster.predict(steps=36, last_window=last_window)

# valores (redondeados a 2 decimales)
pred_1h_value = round(float(pred.iloc[11]), 2)  # 1h
pred_3h_value = round(float(pred.iloc[35]), 2)  # 3h


# timestamps: tu pred puede no traer DatetimeIndex (depende versión),
# así que calculamos nosotros el tiempo futuro desde el último dato real:
t0 = y.index[-1]                       # último timestamp real
t_1h = t0 + pd.Timedelta(hours=1)
t_3h = t0 + pd.Timedelta(hours=3)

# escribo las predicciones en InfluxDB (field name debe ser string)
points = [
    Point(MEASUREMENT).field(OUT_FIELD_1H, pred_1h_value).time(t_1h),
    Point(MEASUREMENT).field(OUT_FIELD_3H, pred_3h_value).time(t_3h),
]

write_api.write(bucket=INFLUX_BUCKET, org=INFLUX_ORG, record=points)

print("OK: predicciones escritas:", pred_1h_value, pred_3h_value)

client.close()
