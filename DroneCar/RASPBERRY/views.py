from django.http import HttpResponse, HttpResponseServerError
#from django.utils import simplejson

import serial
import subprocess
import threading

RBPI_VIDEO_COMMAND= "raspivid -o - --timeout 9999999 --height 240 --rotation 180 --width 320"
VLC_STREAMING_COMMAND= " | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264"
#import subprocess
#import StringIO
#import os		#Sistema operativo
#import sys		#Importarlibreria de sistema
#import glob		#Busqueda en un directorio dando una expresion regular
#import threading	#Import que nos permite ejecutar un nuevo hilo.
#from time import gmtime, strftime	#gmtime es una estructura que te da el tiempo y strftime te permite pasar a string lo que haya en la estruc

#from mugimenduak import *
#from led import *

#from django.core.servers.basehttp import FileWrapper	#El metodo FileWrapper se encarga de devolver el fichero image al navegador para que lo
#from subprocess import call


#JSON_MIMETYPE = "application/json"
#RBPI_VIDEO_COMMAND = "raspvid -o - --timeout 9999999 --height 240 --rotation 180 --width 320"	#comando para hacer que la RBPI se ponga en gra
#VLC_STREAMING_COMMAND = " | cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8090}' :demux=h264"	#lo anterior no es u
#PSEYE0_STREAMING_COMMAND = "gst-launch v4l2src device=/dev/video0 ! video/x-raw-yuv,width=640,height=480,framerate=\(fraction\)30/1 ! jpegenc
#PSEYE1_STREAMING_COMMAND = "gst-launch v412src device=/dev/video1 ! video/x-raw-yuv,width=640,height=480,framerate=\(fraction\)30/1 ! jpegenc
#device_file = None

#def hola(request):
#	return HttpResponse("Hola mundo con Django 1.4")

def musika (request):

	if request.META["REQUEST_METHOD"]!= "GET":
		return HttpResponseServerError()
	else:
		if(not request.GET.has_key("state")):
			return HttpResponseServerError()
		else:
			state=request.GET["state"]
			arduino = serial.Serial('/dev/ttyAMA0',9600)
			if (state=="pp"):
				comando = 'X'
				arduino.write(comando)
				arduino.close()

			if (state=="re"):
				comando = 'Z'
				arduino.write(comando)
				arduino.close()

			if (state=="for"):
				comando = 'C'
				arduino.write(comando)
				arduino.close()

			if (state=="mas"):
				comando = 'M'
				arduino.write(comando)
				arduino.close()

			if (state=="menos"):
				comando = 'N'
				arduino.write(comando)
				arduino.close()
			
			if (state=="stop"):
				comando = 'B'
				arduino.write(comando)
				arduino.close()



def mugimenduak (request):
	
	if request.META["REQUEST_METHOD"]!= "GET":	#comprobamos que lo que nos viene es una peticion GET, sino devolvemos un error
		return HttpResponseServerError()
	else:
		if(not request.GET.has_key("state")):	#comprobamos que la peticion contiene "state" que es lo que deben contener la
			return HttpResponseServerError()
		else:
			state=request.GET["state"]	#cogemos el valor de state que nos llega en la peticion GET, que sera au
			arduino = serial.Serial('/dev/ttyAMA0',9600)
			if (state=="g"):
				comando = 'P'
				arduino.write(comando)
				arduino.close()
				#gelditu()
#				d={}				#creamos un diccionario
#				d["message"]="Robota geldirik"	#creamos una clave message con el valor aurrera.
#				return HttpResponse(simplejson.dumps(d), JSON_MIMETYPE)	#convertimos el diccionario en un archivo j
			
			if (state=="au"):
				comando = 'W'
				arduino.write(comando)
				arduino.close()
#				#aurrerantz()
#				d={}
#				d["message"]="aurrera"
#				return HttpResponse(simplejson.dumps(d), JSON_MIMETYPE)
#
			if (state=="at"):
				comando = 'S'
				arduino.write(comando)
				arduino.close()
#				#atzerantz()
#				d={}
#				d["message"]="atzerantz"
#				return HttpResponse(simplejson.dumps(d), JSON_MIMETYPE)
#
			if (state=="aes"):
				comando = 'E'
				arduino.write(comando)
				arduino.close()
				#eskubirantz()
#				d={}
#				d["message"]="eskubirantz"
#				return HttpResponse(simplejson.dumps(d), JSON_MIMETYPE)

			if (state=="aez"):
				comando = 'Q'
				arduino.write(comando)
				arduino.close()
				#ezkerrerantz()
#				d={}
#				d["message"]="ezkerrerantz"
#				return HttpResponse(simplejson.dumps(d), JSON_MIMETYPE)

			if (state=="ates"):
				comando = 'D'
				arduino.write(comando)
				arduino.close()

			if (state=="atez"):
				comando = 'A'
				arduino.write(comando)
				arduino.close()



def luz(request):
	
	arduino = serial.Serial('/dev/ttyAMA0',9600)
	comando = 'H'
	arduino.write(comando)
	arduino.close()	

def launch_cmd(request):

	command = (RBPI_VIDEO_COMMAND + VLC_STREAMING_COMMAND)
	code = subprocess.call(command, shell=True)
	print code

	

# Create your views here.
