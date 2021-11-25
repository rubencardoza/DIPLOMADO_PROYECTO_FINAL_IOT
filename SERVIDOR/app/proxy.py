import pandas as pd
import pika, os
from influxdb_client import InfluxDBClient, Point, WritePrecision
from datetime import timedelta
import datetime, time
from influxdb_client import InfluxDBClient, Point, Dialect
from influxdb_client.client.write_api import SYNCHRONOUS


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
    presion =  float(vector[1])
    temperatura =  float(vector[0])
    nivel =  float(vector[2])
    print()
    print("RECIBIENDO DATOS......")
    print()
    write_api = client.write_api(write_options=SYNCHRONOUS)
    point = Point("DATOS_FERMENTACION_Y_DESTILACION").field("FECHA", date_time).field("TEMP", temperatura).field("PRES", presion).field("NIV", nivel)
    write_api.write(my_bucket, my_org, point)
    print()
    print("ENVIANDO DATOS  A INFLUXDB---->Temperatura->", temperatura," Presion->",presion,"Nivel--->",nivel)
    print()
    #solicitar_datos_de_influxdb()
    return

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
