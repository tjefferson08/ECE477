from django.contrib import admin
from polls.models import Poll
from polls.models import Choice

# Register your models here.
class ChoiceInline(admin.TabularInline):
    model = Choice
    extra = 3

class PollAdmin(admin.ModelAdmin):
    fieldsets = [ (None, {'fields':['question']}),
                  ('Date info', {'fields':['pub_date'], 'classes':['collapse']}),
                   ]
    inlines = [ChoiceInline]
    list_display = ('question', 'pub_date', 'was_published_recently')
    list_filter = ['pub_date']

admin.site.register(Poll, PollAdmin)

