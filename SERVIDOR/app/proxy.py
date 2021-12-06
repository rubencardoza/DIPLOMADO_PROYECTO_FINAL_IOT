import pandas as pd
import pika, os
from influxdb_client import InfluxDBClient, Point, WritePrecision
from datetime import timedelta
import datetime, time
from influxdb_client import InfluxDBClient, Point, Dialect
from influxdb_client.client.write_api import SYNCHRONOUS

########################
from pandas.core.frame import DataFrame
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression




################################# CREDENCIALES A RABBIT E INFLUXDB EN PROXI########################
my_bucket = os.environ.get("DOCKER_INFLUXDB_INIT_BUCKET")
db_token = os.environ.get("DOCKER_INFLUXDB_INIT_ADMIN_TOKEN")
my_org = os.environ.get("DOCKER_INFLUXDB_INIT_ORG")
rabbit_user = os.environ.get("RABBIT_USER")
rabbit_password = os.environ.get("RABBIT_PASSWORD")
queue_name  = "mensajes"

############################## CREDENCIALES A RABBIT E INFLUXDB EN JUPYTER ###################################
#my_bucket = "DATOS_PROYECTO_FINAL"
#db_token = "test_token"
#my_org = "test_org"
#rabbit_user = "guest"
#rabbit_password = "guest"
#queue_name  = "mensajes"

############################ ACTIVACION DE ESCRITURA Y SOLICITUD DE DATOS EN INFLUXDB #######################
client = InfluxDBClient(url="http://20.121.64.231:8086", token=db_token, org=my_org)
write_api = client.write_api(write_options=SYNCHRONOUS)
query_api = client.query_api()


################################# FUNCION DE ENVIO Y SOLICITUD A INFLUXDB ########################################
def guardar_datos_influxdb_sin_procesar(a):
    lista = []
    valores = a
    vector =  valores.split(",")
    now = datetime.datetime.now()
    now_local = now - timedelta(hours=5)
    date_time = now_local.strftime('%d/%m/%Y %H:%M:%S')
    
    HF =  int(vector[0])
    MF =  int(vector[1])
    SF =  int(vector[2])
    HD =  int(vector[3])
    MD =  int(vector[4])
    SD =  int(vector[5])
    TM =  float(vector[6])
    MA =  float(vector[7])
    PPM =  float(vector[8])
    TDM =  float(vector[9])
    
    
    
    
    print()
    print("##################### RECIBIENDO DATOS......#########################")
    print()
    write_api = client.write_api(write_options=SYNCHRONOUS)
    point = Point("DATOS FERMENTACION Y DESTILACION").field("FECHA", date_time).field("Horas", HF).field("Minutos", MF).field("Segundos", SF).field("Hora.", HD).field("Minutos.", MD).field("Segundos.", SD).field("TEMPERATURA", TM).field("MILILITROS ALCOHOL", MA).field("PPM ALCOHOL", PPM,).field("TIEMPO DESTILACION {minutos}", TDM)
    write_api.write(my_bucket, my_org, point)
    print()
    print("##################### ENVIANDO DATOS  A INFLUXDB---->")
    print()
    solicitar_datos_de_influxdb()
    return

def solicitar_datos_de_influxdb():
    print("######################### SOLICITAR DATOS ALMACENADOS......#####################")
    print()
    data_frame = query_api.query_data_frame('from(bucket:"DATOS_PROYECTO_FINAL") '
                                            '|> range(start: -60m) '
                                            '|> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") '
                                            '|> keep(columns: ["TEMPERATURA", "MILILITROS ALCOHOL", "PPM ALCOHOL", "FECHA", "TIEMPO DESTILACION {minutos}"])')

    #datos_filtrados = data_frame.to_string()

    print("DATAFRAME DE DATOS SOLICITADOS")
    print()
    data_frame.set_index('FECHA', inplace = True)
    print(data_frame)
    print()
    print()
    print("ANALIZANDO DATOS....")
    print()    
    print("#################### ANALISIS DE DATOS  ######################")
    print()
    
    print("Nuevo_df_influxDB ")
    print()
    Nuevo_df_influxDB = data_frame.loc[:,'MILILITROS ALCOHOL':'TIEMPO DESTILACION {minutos}']
    print(Nuevo_df_influxDB )
    print()
    print("Veracidad de los datos")
    print()   
    df_influxDB_procesado = Nuevo_df_influxDB.fillna(Nuevo_df_influxDB.mean())
    print()
    print("Dataframe para enviar a grafana")
    print(df_influxDB_procesado )
    print()
    print("Veracidad de los datos a grafanas")
    df_veracidad_datos = df_influxDB_procesado.isnull().sum()
    print(df_veracidad_datos)
    print()
    print("MAXIMO,MINIMO,MEDIA,DESVIACION")
    data_max = df_influxDB_procesado.loc[:,'MILILITROS ALCOHOL':'TEMPERATURA'].max()
    data_min = df_influxDB_procesado.loc[:,'MILILITROS ALCOHOL':'TEMPERATURA'].min()
    data_media = df_influxDB_procesado.loc[:,'MILILITROS ALCOHOL':'TEMPERATURA'].mean()
    data_std = df_influxDB_procesado.loc[:,'MILILITROS ALCOHOL':'TEMPERATURA'].std()
    data_resume = DataFrame([data_min, data_max, data_media, data_std], index=['min','max','mean', 'std'])
    print(data_resume)
    print()
    
    #print("REGRESION LINEAL")
    
    
    #x_train = data_frame.loc[:data_frame.shape[0]-1,'TEMPERATURA':'TIEMPO DESTILACION {minutos}']
    #y_train = data_frame.loc[:data_frame.shape[0]-1,'MILILITROS ALCOHOL']
    #predict_value = DataFrame(data_frame[['TEMPERATURA', 'TIEMPO DESTILACION {minutos}']].values[-1], index=['TEMPERATURA', 'TIEMPO DESTILACION {minutos}'])
    #print(predict_value.shape ,type(predict_value))
    #print(predict_value)
    #linear_reg = LinearRegression().fit(x_train, y_train)
    #test_predict = linear_reg.predict(predict_value.T)
    #print("prediccion de la temperatura con respecto a las otras varibles ", test_predict)
    
    

    print("#################### ENVIO DE DATOS  ######################")
    print()
    print("DATOS FILTRADOS...")
    print()
    point2 = Point("DATOS_DESTILACION_PROCESADOS").field("TEMPERATURA", float(data_frame['TEMPERATURA'].values[-1])).field("MILILITROS ALCOHOL", float(data_frame['MILILITROS ALCOHOL'].values[-1])).field("PPM ALCOHOL", float(data_frame['PPM ALCOHOL'].values[-1]))
    write_api.write("DESTILACION", my_org, point2)
    print()
    print("ENVIO DE DATOS PROCESADOS CON EXITO---->BUCKET---FERMENTACION,DESTILACION.")
    print()
    print()
    #client.close()
    return


###############################################  ACTUALIZACION DE DATOS ###################################

def process_function(msg):
  mesage = msg.decode("utf-8")
  #print(mesage)
  #enviar_DB(mesage)
  guardar_datos_influxdb_sin_procesar(mesage)
  return


while 1:

  url = os.environ.get('CLOUDAMQP_URL', 'amqp://{}:{}@20.121.64.231:5672/%2f'.format(rabbit_user, rabbit_password))
  params = pika.URLParameters(url)
  connection = pika.BlockingConnection(params)
  channel = connection.channel() # start a channel
  channel.queue_declare(queue=queue_name) # Declare a queue
  channel.queue_bind(exchange="amq.topic", queue=queue_name, routing_key='#')
  # create a function which is called on incoming messages
  def callback(ch, method, properties, body):
    process_function(body)

  # set up subscription on the queue
  channel.basic_consume(queue_name, callback, auto_ack=True)

  #start consuming (blocks)
  channel.start_consuming()
  connection.close()
################################################################################################################
