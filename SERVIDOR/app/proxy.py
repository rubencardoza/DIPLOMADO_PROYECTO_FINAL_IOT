# example_consumer.py
import pika, os, csv
from influxdb_client import InfluxDBClient, Point, WritePrecision
from influxdb_client.client.write_api import SYNCHRONOUS
from datetime import datetime

my_bucket = os.environ.get("DOCKER_INFLUXDB_INIT_BUCKET")
db_token = os.environ.get("DOCKER_INFLUXDB_INIT_ADMIN_TOKEN")
my_org = os.environ.get("DOCKER_INFLUXDB_INIT_ORG")
rabbit_user = os.environ.get("RABBIT_USER")
rabbit_password = os.environ.get("RABBIT_PASSWORD")
queue_name  = "mensajes"

client = InfluxDBClient(url="http://influxdb:8086", token=db_token, org=my_org)
write_api = client.write_api(write_options=SYNCHRONOUS)
query_api = client.query_api()

def enviar_DB(a):
    lista = []
    valores = a
    vector =  valores.split(",")
    presion =  float(vector[0])
    temperatura =  float(vector[1])
    nivel =  float(vector[2])
    write_api = client.write_api(write_options=SYNCHRONOUS)
    point = Point("Mediciones").field("TEMPERATURA", temperatura).time(datetime.utcnow(),WritePrecision.NS).field("PRESION", presion).time(datetime.utcnow(),WritePrecision.NS).field("NIVEL", nivel).time(datetime.utcnow(), WritePrecision.NS)
    write_api.write(my_bucket, my_org, point)
    return

def process_function(msg):
  mesage = msg.decode("utf-8")
  print(mesage)
  enviar_DB(mesage)
  return

while 1:

  url = os.environ.get('CLOUDAMQP_URL', 'amqp://{}:{}@rabbit:5672/%2f'.format(rabbit_user, rabbit_password))
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

  # start consuming (blocks)
  channel.start_consuming()
  connection.close()
