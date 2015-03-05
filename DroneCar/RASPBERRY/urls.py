from django.conf.urls import patterns, include, url
#from django.contrib import admin

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'proyecto_web.views.home', name='home'),
    # url(r'^blog/', include('blog.urls')),

    url(r'^hola/$', 'hola_mundo.views.hola'),
    url(r'^mugimenduak/$', 'hola_mundo.views.mugimenduak'),
    url(r'^luz/$', 'hola_mundo.views.luz'),
    url(r'^musika/$', 'hola_mundo.views.musika'),
    url(r'^launch_cmd/$', 'hola_mundo.views.launch_cmd'),	
)
