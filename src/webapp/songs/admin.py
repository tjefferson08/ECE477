from django.contrib import admin
from songs.models import Song

# Register your models here.
class SongAdmin(admin.ModelAdmin):
    fieldsets = [ (None, {'fields':['title', 'artist', 'album']}),]
   # list_display = ('question', 'pub_date', 'was_published_recently')
    #list_filter = ['pub_date']

admin.site.register(Song, SongAdmin)

