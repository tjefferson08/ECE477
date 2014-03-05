from django.conf.urls import patterns, include, url

from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
                       url(r'^songs/', include('songs.urls', namespace="songs")),
                       url(r'^admin/', include(admin.site.urls)),
                       )
