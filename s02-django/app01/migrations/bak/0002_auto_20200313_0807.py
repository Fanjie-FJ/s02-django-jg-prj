# Generated by Django 2.1 on 2020-03-13 00:07

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('app01', '0001_initial'),
    ]

    operations = [
        migrations.CreateModel(
            name='Register',
            fields=[
                ('devid', models.AutoField(primary_key=True, serialize=False)),
                ('siteid', models.CharField(max_length=32)),
                ('address', models.CharField(max_length=32)),
            ],
        ),
        migrations.DeleteModel(
            name='Employee',
        ),
    ]
