# Prediction Pipeline

This script performs the automated inference of the PM2.5 forecasting model.

The pipeline runs periodically and performs the following steps:

1. Retrieve recent PM2.5 measurements from InfluxDB
2. Preprocess the time series data
3. Load the trained Random Forest forecasting model
4. Generate predictions for 1 hour and 3 hours ahead
5. Write predictions back to the database for visualization in Grafana
