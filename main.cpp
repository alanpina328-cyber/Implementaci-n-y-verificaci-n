#include <iostream>
#include <string>
using namespace std;

enum Estado { APAGADO, ACTIVO, ERROR };

struct Ubicacion {
    string zona;
    int posicion;
};

struct Sensor {
    int id;
    Estado estado;
    Ubicacion ubicacion;
    double lectura;
    unsigned banderas;
};

struct Indicadores {
    unsigned energia : 1;
    unsigned conexion : 1;
    unsigned alarma : 1;
    unsigned modo : 2;
};

const unsigned BIT_ALARMA = 2;
const unsigned MASCARA_ALARMA = 1u << BIT_ALARMA;

class Sistema {
public:
    Sensor sensores[3] = {
        {101, ACTIVO, {"Laboratorio", 1}, 24.5, 0u},
        {102, ERROR, {"Laboratorio", 2}, 91.0, 0u},
        {103, APAGADO, {"Pasillo", 1}, 0.0, 0u}
    };

    void mostrarResumen() const { //Recorre el arreglo y muestra los datos de cada sensor
        for (int i = 0; i < 3; i++) {
            const Sensor& s = sensores[i];
            cout << "ID " << s.id << "  " << s.ubicacion.zona
                 << "  posicion " << s.ubicacion.posicion
                 << "  lectura " << s.lectura << "\n";
        }
    }

    int contarActivos() const {//Cuenta los sensores cuyo estado sea ACTIVO
        int total = 0;
        for (int i = 0; i < 3; i++) {
            if (sensores[i].estado == ACTIVO) {
                total++;
            }
        }
        return total;
    }

    bool tieneBanderas(int id, unsigned mascara) const {// Consulta mediante máscara
        for (int i = 0; i < 3; i++) {
            if (sensores[i].id == id) {
                return (sensores[i].banderas & mascara) != 0;
            }
        }
        return false;
    }

    void activarBanderas(int id, unsigned bit) {// Construye máscara con operador &
        for (int i = 0; i < 3; i++) {
            if (sensores[i].id == id) {
                sensores[i].banderas |= (1u << bit);
            }
        }
    }

    Indicadores construirIndicadores() const {//Asigna los 5 bits del objeto panel
        Indicadores panel{};
        panel.energia = 1;
        panel.conexion = 1;
        panel.alarma = tieneBanderas(101, MASCARA_ALARMA) ? 1 : 0;
        panel.modo = 2;
        return panel;
    }
};

int main() {
    Sistema sistema;
    sistema.mostrarResumen();

    cout << "\nSensores activos: " << sistema.contarActivos() << "\n";

    cout << "Sensor 102 con alarma: "
         << (sistema.tieneBanderas(102, MASCARA_ALARMA) ? "NO" : "SI") << "\n";

    sistema.activarBanderas(101, BIT_ALARMA);
    cout << "Despues de activar:\n";
    cout << "Sensor 101 con alarma: "
         << (sistema.tieneBanderas(101, MASCARA_ALARMA) ? "SI" : "NO") << "\n";

    Indicadores panel = sistema.construirIndicadores();
    cout << "\nPanel: energia=" << panel.energia
         << ", conexion=" << panel.conexion
         << ", alarma=" << panel.alarma
         << ", modo=" << panel.modo << "\n";

    return 0;
}

 
