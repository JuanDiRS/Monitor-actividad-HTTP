#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <PcapLiveDeviceList.h>
#include <SystemUtils.h>
#include <memory>
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include <PcapFileDevice.h>

//obtiene el protocolo del paquete y lo convierte a string.
std::string getProtocolTypeAsString(pcpp::ProtocolType protocolType) {
	switch (protocolType) {
	case pcpp::Ethernet:
		return "Ethernet";
	case pcpp::IPv4:
		return "IPv4";
	case pcpp::TCP:
		return "TCP";
	case pcpp::HTTPRequest:
	case pcpp::HTTPResponse:
		return "HTTP";
	default:
		return "Unknown";
	}
}

//hace un conteo simple de todos paquetes capturados
struct PacketStats {
	int conteopkt = 0;
	int conthttp = 0;

	//se usa map para guardar cada paquete que llega
	//map permite guardar claves y valores, Cada clave ES UNICA y se pude usar para acceder al valor
	std::map<std::string, int>hostsDetectados;

	//Un vector que almacene los resultyados de map
	std::vector<std::pair<std::string, int>> rank;

	//Se suma el contador por cada paquete que llega
	void consumePacket(const pcpp::Packet& packet, const std::string *host) {
		conteopkt++;
		if (host != nullptr) {
			conthttp++;
			hostsDetectados[*host]++;
		}
	}

	//Muestra los resultados  del conteo en consola
	void printToConsole(){
		//recorre map y los resultados los alamacena en el vector
		for (const auto& [clave, valor] : hostsDetectados) {
			rank.emplace_back(clave, valor);
		}

		// Ordena el valor del contador de map de mayor a menor
		std::sort(rank.begin(), rank.end(), [](const auto& a, const auto& b) {
			return a.second > b.second;
			});
		// recorre e imprime el vector
		for (const auto [clave,valor]: rank){
			std::cout <<clave << "\t" << "\t" << valor << std::endl;
		}
		
		std::cout << "Fueron: " << conteopkt << " paquetes en total." << std::endl;
		std::cout << "Fueron: " << conthttp << " http en total." << std::endl;
	}
};

//Esta es la funcion callback, la que cada vez que llega un paquete se llama esta funcion, esta solo funciona para metodos asincronios, metodos que no se ejecutan en el main.
//raw packet, el paquete en bruto sin procesar
static void onPacketArrives(pcpp::RawPacket* packet, pcpp::PcapLiveDevice *dev, void *cookie) {

	//extrae el obgeto stats desde cookie
	auto *stats = static_cast<PacketStats*>(cookie);

	//analiza el paquete en bruto
	pcpp::Packet parsedPacket(packet);

	//El obgeto requestLayrer esta contenido en un paquete de la libreria, aqui se accede a este paquete mediante un puntero sin necesidad de copiarlo
	auto *httpRequestLayer = parsedPacket.getLayerOfType<pcpp::HttpRequestLayer>();
	if (httpRequestLayer != nullptr) {
		auto *hostField = httpRequestLayer->getFieldByName(PCPP_HTTP_HOST_FIELD);
		if (hostField != nullptr){
			std::string nombrehost = hostField->getFieldValue();
			stats->consumePacket(parsedPacket, &nombrehost);
		}
	}

}




int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::cerr << "No se ingreso el tiempo de ejecucion" << std::endl;
		return 1;
	}
	//convierto lo que hay en entero
	int tiempo = std::atoi(argv[1]);
	//se cambio de tomar informacion de una IP a una interfaz del equipo en este caso any que toma todo el trafico que hay en linux wsl.
	std::string interfaceName = "any";
	auto *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(interfaceName);
	if (dev == nullptr) {
		std::cerr << "No se encontro la interfaz'" << interfaceName << "'" << std::endl;
		return 1;
	}
	// Informacion de la interfaz
	std::cout
		<< "Interface info:" << std::endl
		<< "   Interface name:        " << dev->getName() << std::endl
		<< "   Interface description: " << dev->getDesc() << std::endl
		<< "   MAC address:           " << dev->getMacAddress() << std::endl
		<< "   Default gateway:       " << dev->getDefaultGateway() << std::endl
		<< "   Interface MTU:         " << dev->getMtu() << std::endl;

	if (!dev->getDnsServers().empty()) {
		std::cout << "   DNS server:            " << dev->getDnsServers().front() << std::endl;
	}
	// open the device before start capturing/sending packets
	if (!dev->open())
	{
		std::cerr << "Cannot open device" << std::endl;
		return 1;
	}
	// crea el obgeto stats para instanciar la esttructura
	PacketStats stats;
	std::cout << std::endl << "Iniciando captura de paquetes...." << std::endl;

	//se eligio el modo de captura asincronica ya que entrega los resultados inmediatamente.
	//se inicia la captura asincronica se da la funcion callback y las stacks de esta se ejecuntan como la coockie

	dev->startCapture(onPacketArrives, &stats);

	// Duerme por el tiempo que se elija en segundos el main miesyras se capturan y analizan los paquetes.
	pcpp::multiPlatformSleep(tiempo);

	//cuando se despierta la fucnion main se detiene la capturad e paquetes
	dev->stopCapture();

	// muestra los resultados
	std::cout << "Resultados:" << std::endl;
	stats.printToConsole();
}
