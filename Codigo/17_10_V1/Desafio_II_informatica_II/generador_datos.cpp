/*#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    cout << "--- GENERADOR DE DATOS SINTETICOS (UDE@BOOK) ---" << endl;

    int totalUsuarios = 1200; // Supera el requisito de mínimo 1000 usuarios[cite: 8, 29, 44]

    // 1. Generar usuarios.txt
    ofstream archU("usuarios.txt");
    if (!archU.is_open()) {
        cout << "[Error] No se pudo crear usuarios.txt" << endl;
        return 1;
    }
    for (int i = 0; i < totalUsuarios; i++) {
        archU << i << ";usuario_" << i << ";15/05/2000;user" << i << "@udea.edu.co;"
              << "/users/x/usuario_" << i << "/albums/profile.png;pass123;Medellin;Colombia;01/01/2026\n";
    }
    archU.close();
    cout << "[OK] usuarios.txt generado con " << totalUsuarios << " registros." << endl;

    // 2. Generar amistades.txt (Red densa para garantizar rangos de 1x, 2.5x y 3.5x en amigos en comun)
    ofstream archA("amistades.txt");
    if (!archA.is_open()) {
        cout << "[Error] No se pudo crear amistades.txt" << endl;
        return 1;
    }
    for (int i = 0; i < totalUsuarios; i++) {
        // Conectar cada usuario con un rango amplio de vecinos
        for (int j = i + 1; j < i + 35 && j < totalUsuarios; j++) {
            if (rand() % 3 != 0) { // ~66% de probabilidad de conexión
                archA << i << ";" << j << "\n";
            }
        }
    }
    archA.close();
    cout << "[OK] amistades.txt generado." << endl;

    // 3. Generar solicitudes.txt
    ofstream archS("solicitudes.txt");
    if (!archS.is_open()) {
        cout << "[Error] No se pudo crear solicitudes.txt" << endl;
        return 1;
    }
    for (int i = 10; i < 30; i++) {
        archS << i << ";" << (i + 40) << "\n";
    }
    archS.close();
    cout << "[OK] solicitudes.txt generado." << endl;

    // 4. Generar mensajes.txt (Fechas distribuidas entre septiembre y octubre de 2026 -> > 6 semanas)[cite: 8, 29, 44]
    ofstream archM("mensajes.txt");
    if (!archM.is_open()) {
        cout << "[Error] No se pudo crear mensajes.txt" << endl;
        return 1;
    }
    int dias[6] = {1, 10, 20, 25, 28, 30};
    for (int i = 0; i < 800; i++) {
        int emisor = rand() % 50;
        int receptor = rand() % 50;
        if (emisor != receptor) {
            int d = dias[i % 6];
            int m = (i % 2 == 0) ? 9 : 10; // Septiembre (09) y Octubre (10)
            archM << emisor << ";" << receptor << ";"
                  << (d < 10 ? "0" : "") << d << "/"
                  << (m < 10 ? "0" : "") << m << "/2026;"
                  << (i % 2) << ";Hola este es un mensaje automatizado de prueba numero " << i << "\n";
        }
    }
    archM.close();
    cout << "[OK] mensajes.txt generado con rango de fechas de mas de 6 semanas." << endl;

    cout << "\n¡Todos los archivos de prueba masiva creados con exito!" << endl;
    return 0;
}*/