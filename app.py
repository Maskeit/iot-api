from flask import Flask, request, jsonify

app = Flask(__name__)

# PIN autorizado para prueba
PIN_CORRECTO = "4444"

# endpoint que recibe las solicitudes http
@app.route("/api/verificar_pin", methods=["POST"])
def verificar_pin():
    data = request.json
    pin = data.get("pin")

    if pin == PIN_CORRECTO:
        return "1"  # Autorizado
    else:
        return "0"  # No autorizado

if __name__ == "__main__":
    app.run(port=5000) # este puerto se le pasa a ngro
