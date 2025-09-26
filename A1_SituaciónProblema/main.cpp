#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// Función para leer los archivos

string leerArchivo(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string contenido, linea;
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir " << nombreArchivo << endl; //Para evitar problemas al momento de leer los archivos
        return "";
    }
    while (getline(archivo, linea)) { // Aquí se lee el archivo linea por linea
        contenido += linea;
    }
    return contenido;
}

// Primera función para buscar un patron usando KMP

int buscarPatron(const string& texto, const string& patron) {
    if (patron.empty()) return -1;

    int n = texto.size();
    int m = patron.size();

    // aqui se hace el arreglo o tabla LPS 
    vector<int> lps(m, 0);
    int len = 0;   // longitud prefijo más largo
    int i = 1;     

    while (i < m) {
        if (patron[i] == patron[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1]; // retrocede en  el patron
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }

    // Buscar el patron usando la tabla LPS
    i = 0;  // indice del texto
    int j = 0;  // indice pque se usa para el patron

    while (i < n) {
        if (texto[i] == patron[j]) {
            i++;
            j++;
        }

        if (j == m) {
            // Patron encontrado, devolver posicion
            return i - j;
        } else if (i < n && texto[i] != patron[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }

    return -1; //  en caso de que no encuentre el patrón regresa -1
}

// Aquí es la función de buscar el Palindromo 
pair<int,int> encontrarPalindromoMasLargo(const string& s) {
    if (s.empty()) return {1, 1};
    string t = "#";
    for (char c : s) { t += c; t += '#'; }
    int n = t.size(), center = 0, right = 0, max_len = 0, max_center = 0;
    vector<int> p(n, 0);
    for (int i = 0; i < n; i++) {
        if (i < right) p[i] = min(right - i, p[2*center - i]);
        while (i-p[i]-1 >= 0 && i+p[i]+1 < n && t[i-p[i]-1] == t[i+p[i]+1]) p[i]++;
        if (i+p[i] > right) { center = i; right = i+p[i]; }
        if (p[i] > max_len) { max_len = p[i]; max_center = i; }
    }
    int inicio = (max_center - max_len) / 2 + 1;
    int fin = inicio + max_len - 1;
    return {inicio, fin};
}

// Aquí se hace uso de la programación dinamica para encontrar la substring más grande entre los dos archivos 
pair<int,int> encontrarSubstringComun(const string& s1, const string& s2) {
    int n = s1.length();
    int m = s2.length();
    int max_len = 0;
    int end_index = 0; 
    
    // Tabla que se usa en la programación dinámica
    vector<vector<int>> dp(n+1, vector<int>(m+1, 0));
    
    // En este for se llena la tabla
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (s1[i-1] == s2[j-1]) {
                dp[i][j] = dp[i-1][j-1] + 1;
                if (dp[i][j] > max_len) {
                    max_len = dp[i][j];
                    end_index = i - 1; // posición final en s1 (0-indexado)
                }
            } else {
                dp[i][j] = 0; // esta parte es para si no hay coincidencia, se reinicia para seguir buscando 
            }
        }
    }
    
    if (max_len == 0) return {1, 0}; // Esta es por si no hay ninguna coincidencia 
    
   

    int inicio = end_index - max_len + 2; 
    int fin = end_index + 1; 
    return {inicio, fin};
}

// Amigos, le agregue tipos mensaje de retroalimentación para que no salga todo confuso en la terminal, asi se entiende mejor las salidas
int main() {
    string transmission1 = leerArchivo("transmission01.txt");
    string transmission2 = leerArchivo("transmission02.txt");
    string mcode1 = leerArchivo("mcode01.txt");
    string mcode2 = leerArchivo("mcode02.txt");
    string mcode3 = leerArchivo("mcode03.txt");

    // Aqui se buscan los patrones 
    vector<pair<string,string>> busquedas = {
        {transmission1, mcode1}, {transmission1, mcode2}, {transmission1, mcode3},
        {transmission2, mcode1}, {transmission2, mcode2}, {transmission2, mcode3}
    };

    
    cout << "d BUSQUEDA DE CODIGOS MALICIOSOS " << endl;
    vector<string> nombres = {"mcode1", "mcode2", "mcode3", "mcode1", "mcode2", "mcode3"};
    vector<string> transmisiones = {"transmission1", "transmission1", "transmission1", 
                                   "transmission2", "transmission2", "transmission2"};
    
    for (int i = 0; i < busquedas.size(); i++) {
        int pos = buscarPatron(busquedas[i].first, busquedas[i].second);
        cout << "Buscando " << nombres[i] << ".txt en " << transmisiones[i] << ".txt: ";
        if (pos != -1) {
            cout << "true " << (pos + 1) << " (encontrado en posicion " << (pos + 1) << ")" << endl;
        } else {
            cout << "false (no encontrado)" << endl;
        }
    }

   // Aquí Fredy pon lo de la llamada a la función de Palindromos, la que hiciste 
    cout << "\n BUSQUEDA DE CODIGO ESPEJEADO O PLAINDROMOS " << endl;
    auto p1 = encontrarPalindromoMasLargo(transmission1);
    auto p2 = encontrarPalindromoMasLargo(transmission2);
    cout << "Palindromo mas largo en transmission1.txt: posicion " << p1.first << " a " << p1.second << endl;
    cout << p1.first << " " << p1.second << endl;
    cout << "Palindromo mas largo en transmission2.txt: posicion " << p2.first << " a " << p2.second << endl;
    cout << p2.first << " " << p2.second << endl;

    
    // Aquí se imprime las posiciones del inicio y fin de la cadena más larga, pero en la función le puse que era el indice +1 por lo que 
    // si es la posición real, no hay que restarle nada.
    cout << "\n ANÁLISIS DE SIMILITUD ENTRE LOS TEXTOS " << endl;
    auto sub = encontrarSubstringComun(transmission1, transmission2);
    cout << "Substring comun más largo entre transmission1.txt y transmission2.txt:" << endl;
    cout << "Posicion en transmission1.txt: " << sub.first << " a " << sub.second << endl;
    cout << sub.first << " " << sub.second << endl;

    return 0;
}
