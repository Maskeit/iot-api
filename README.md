# Aplicación IoT para registro y autenticación mediante huella dactilar

## Para la aplicación de python se require lo siguiente

* Tener `Python 3.12.7` o superior
* Tener ngrok descargado `ngrok version 3.9.0` 
### Crear un entorno virual
<pre><code> python -m venv nombre_del_env</code></pre>

### Activar el entorno 
<pre><code> source nombre_del_env/bin/activate </code></pre>

### Instalar las dependencias
<pre><code> pip install -r requirements.txt </code></pre>

### Ejecutar la aplicación Flask

<pre><code> python app.py</code></pre>

### Ejecutar Ngrok para que el puerto este abierto a la web pública
Esto expondrá el puerto 5000 de localhost`
<pre><code> ngrok http 5000 </code></pre>


## Para la aplicación de C se require lo siguiente
Copiar el codigo del archivo `main.c` en la plataforma de particle Web IDE

### Agregar las librerias manualmente de 
* HttpClient.h
* LiquidCrystal_I2C_Spark.h
### Compilar el programa y si todo esta correcto, flashearlos

Verificar la siguiente linea, ya que contiene una `url` que apunta al servidor de Flask, pero como apunta con ngrok, cada vez que se ejecuta ngrok, hay que modificar la asignacion de la variable `request.hostname` será la que ngrok te asigne.
<pre><code> request.hostname = "https://5a68-187-192-253-153.ngrok-free.app/"; // Cambia esto por tu URL</code></pre>

