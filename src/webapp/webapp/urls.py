from django.conf.urls import patterns, include, url
from django.http import HttpResponseRedirect
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
                       url(r'^songs/', include('songs.urls', namespace="songs")),
                       url(r'^$', lambda x: HttpResponseRedirect('/songs/')),
                       url(r'^admin/', include(admin.site.urls)),
                       )
