## Complejidad del codigo que genera los histogramas
el codigo el cual se encarga de realizar esta tarea se encuentra en la funcion "Print to console" la cual almacena los paquetes ya analizados y los cuenta por url y finalmente muestra el histograma y las estadisticas de paquetes encontrados:

void printToConsole() {
    // Copiar datos del map al vector: O(n)
    for (const auto& [clave, valor] : hostsDetectados) {
        rank.emplace_back(clave, valor);
    }

    // Ordenar el vector por número de peticiones (valor): O(n log n)
    std::sort(rank.begin(), rank.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });

    // Imprimir los resultados: O(n)
    for (const auto [clave, valor] : rank) {
        std::cout << clave << "\t\t" << valor << std::endl;
    }

    // Imprimir conteo total (2 líneas): O(1)
    std::cout << "Fueron: " << conteopkt << " paquetes en total." << std::endl;
    std::cout << "Fueron: " << conthttp << " http en total." << std::endl;
}
