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
client = InfluxDBClient(url="http://52.149.151.75:8086", token=db_token, org=my_org)
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
    
    
    
    
    print()
    print("##################### RECIBIENDO DATOS......#########################")
    print()
    write_api = client.write_api(write_options=SYNCHRONOUS)
    point = Point("DATOS_FERMENTACION_Y_DESTILACION").field("FECHA", date_time).field("F_Horas", HF).field("F_Minutos", MF).field("F_Segundos", SF).field("D_Horas", HD).field("D_Minutos", MD).field("D_Segundos", SD).field("TEMPERATURA", TM).field("MILILITROS_ALCOHOL", MA).field("PPM_ALCOHOL", PPM)
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
                                            '|> keep(columns: ["TEMPERATURA", "MILILITROS_ALCOHOL", "PPM_ALCOHOL", "FECHA"])')

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

    print("#################### ENVIO DE DATOS  ######################")
    print()
    print("DATOS FILTRADOS...")
    print()
    point2 = Point("DATOS_DESTILACION_PROCESADOS").field("TEMPERATURA", float(data_frame['TEMPERATURA'].values[-1])).field("MILILITROS_ALCOHOL", float(data_frame['MILILITROS_ALCOHOL'].values[-1])).field("PPM_ALCOHOL", float(data_frame['PPM_ALCOHOL'].values[-1]))
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

  url = os.environ.get('CLOUDAMQP_URL', 'amqp://{}:{}@52.149.151.75:5672/%2f'.format(rabbit_user, rabbit_password))
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

