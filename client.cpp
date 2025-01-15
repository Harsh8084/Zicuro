


// #include <iostream>
// #include <fstream>
// #include <cstring>
// #include <winsock2.h>
// #include <ws2tcpip.h>
// #include <vector>
// #include <map>

// using namespace std;

// // Structure to represent a packet
// struct Packet {
//     char symbol[5]; // 4 bytes + null terminator
//     char buysellindicator;
//     uint32_t quantity;
//     uint32_t price;
//     uint32_t packetSequence;
// };

// // Function to create a payload for "Resend Packet" request
// vector<char> createResendRequest(uint32_t seqNum) {
//     vector<char> payload(2);
//     payload[0] = 2; // Call Type 2
//     payload[1] = seqNum; // Sequence number to resend
//     return payload;
// }

// // Function to parse a packet from received data
// Packet parsePacket(const char* data) {
//     Packet packet;
//     memcpy(packet.symbol, data, 4);
//     packet.symbol[4] = '\0'; // Null-terminate the symbol
//     packet.buysellindicator = data[4];
//     packet.quantity = ntohl(*(uint32_t*)(data + 5));
//     packet.price = ntohl(*(uint32_t*)(data + 9));
//     packet.packetSequence = ntohl(*(uint32_t*)(data + 13));
//     return packet;
// }

// int main() {
//     // Initialize Winsock
//        WSADATA wsaData;
//     if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//         std::cerr << "WSAStartup failed." << std::endl;
//         return 1;
//     }
   
//     int sock = socket(AF_INET, SOCK_STREAM, 0);
//     if (sock == -1) {
//         cerr << "Error creating socket." << endl;
//         return 1;
//     }
//        sockaddr_in serverAddr={};
//     serverAddr.sin_family = AF_INET;
//     serverAddr.sin_port = htons(3000); 
//     serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");


//     // 3. Connect to the server
//     if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
//         cerr << "Error connecting to server." << endl;
//         return 1;
//     }
//     else {
//     cout << "Connected to the server." << endl;  // Add this line to check connection
// }
   
//     // 4. Send "Stream All Packets" request
// char streamRequest[] = {1, 0}; // Request all packets (Call Type 1)
// send(sock, streamRequest, sizeof(streamRequest), 0); 
//     // char streamRequest = 1; // Call Type 1
//     //  send(sock, &streamRequest, 1, 0);
//     // 5. Receive and process packets
//     char buffer[1024]; // Packet size
//     vector<Packet> receivedPackets;
//     map<uint32_t, bool> receivedSeqs; 
   

//    while (true) {
//     int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
//     if (bytesRead <= 0) {
//         cerr << "Error or no data received." << endl;
//         break; // Exit if no data is received
//     }

//     // Debugging output for received data
//    else {
//     cout << "Received " << bytesRead << " bytes." << endl;  // Add this line to check the bytes received
// }

//     for (int i = 0; i < bytesRead; ++i) {
//         cout << hex << (int)(unsigned char)buffer[i] << " ";
//     }
//     cout << endl;

//     // Parse the packet
//     Packet packet = parsePacket(buffer);
//     receivedPackets.push_back(packet);
//     receivedSeqs[packet.packetSequence] = true;
// }


//     // 6. Check for missing packets and resend requests
//     // uint32_t expectedSeq = 1;
//     // for (Packet& packet : receivedPackets) {
//     //     while (expectedSeq < packet.packetSequence) {
//     //         if (!receivedSeqs[expectedSeq]) {
//     //             vector<char> resendRequest = createResendRequest(expectedSeq);
//     //             send(sock, resendRequest.data(), resendRequest.size(), 0);
//     //             recv(sock, buffer, sizeof(buffer), 0);
//     //             Packet resentPacket = parsePacket(buffer);
//     //             receivedPackets.push_back(resentPacket);
//     //             receivedSeqs[resentPacket.packetSequence] = true;
//     //         }
//     //         expectedSeq++;
//     //     }
//     //     expectedSeq++;
//     // }

//     // 7. Generate JSON output
//     ofstream jsonFile("D:/HFT/abx_exchange_server/order_book.json");
//     if (!jsonFile) {
//     cerr << "Error opening JSON file for writing." << endl;
//     return 1;
//      }
//     jsonFile << "["; // Start of JSON array

//     for (size_t i = 0; i < receivedPackets.size(); ++i) {
//         Packet& packet = receivedPackets[i];
//         jsonFile << "{";
//         jsonFile << "\"symbol\":\"" << packet.symbol << "\",";
//         jsonFile << "\"buysellindicator\":\"" << packet.buysellindicator << "\",";
//         jsonFile << "\"quantity\":" << packet.quantity << ",";
//         jsonFile << "\"price\":" << packet.price << ",";
//         jsonFile << "\"packetSequence\":" << packet.packetSequence;
//         jsonFile << "}";
//         if (i < receivedPackets.size() - 1) {
//             jsonFile << ",";
//         }
//     }

//     jsonFile << "]"; // End of JSON array
//     jsonFile.close();

//     // 8. Close the connection and clean up Winsock
//     closesocket(sock);
//     WSACleanup();

//     return 0;
// }



#include <iostream>
#include <fstream>
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <map>

using namespace std;

// Structure to represent a packet
struct Packet {
    char symbol[5]; // 4 bytes + null terminator
    char buysellindicator;
    uint32_t quantity;
    uint32_t price;
    uint32_t packetSequence;
};

// Function to create a payload for "Resend Packet" request
vector<char> createResendRequest(uint32_t seqNum) {
    vector<char> payload(2);
    payload[0] = 2; // Call Type 2
    payload[1] = static_cast<char>(seqNum); // Sequence number to resend
    return payload;
}

// Function to parse a packet from received data
Packet parsePacket(const char* data) {
    Packet packet;
    memcpy(packet.symbol, data, 4);
    packet.symbol[4] = '\0'; // Null-terminate the symbol
    packet.buysellindicator = data[4];
    packet.quantity = ntohl(*(uint32_t*)(data + 5));
    packet.price = ntohl(*(uint32_t*)(data + 9));
    packet.packetSequence = ntohl(*(uint32_t*)(data + 13));
    return packet;
}

int main() {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Error creating socket." << endl;
        WSACleanup();
        return 1;
    }

         sockaddr_in serverAddr={};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(3000); 
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Error connecting to server." << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }
    cout << "Connected to the server." << endl;

    // Send "Stream All Packets" request
    char streamRequest[] = {1, 0}; // Request all packets (Call Type 1)
    if (send(sock, streamRequest, sizeof(streamRequest), 0) == SOCKET_ERROR) {
        cerr << "Error sending Stream All Packets request." << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Receive and process packets
    char buffer[1024];
    vector<Packet> receivedPackets;
    map<uint32_t, bool> receivedSeqs;

    while (true) {
        int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            break; // Exit loop if no data is received or error occurs
        }

        cout << "Received " << bytesRead << " bytes." << endl;

        // Parse and store packets
        Packet packet = parsePacket(buffer);
        receivedPackets.push_back(packet);
        receivedSeqs[packet.packetSequence] = true;
    }

    // Check for missing packets and request them
    uint32_t expectedSeq = 1;
    for (Packet& packet : receivedPackets) {
        while (expectedSeq < packet.packetSequence) {
            if (!receivedSeqs[expectedSeq]) {
                vector<char> resendRequest = createResendRequest(expectedSeq);
                send(sock, resendRequest.data(), resendRequest.size(), 0);
                int bytesRead = recv(sock, buffer, sizeof(buffer), 0);
                if (bytesRead > 0) {
                    Packet resentPacket = parsePacket(buffer);
                    receivedPackets.push_back(resentPacket);
                    receivedSeqs[resentPacket.packetSequence] = true;
                }
            }
            expectedSeq++;
        }
        expectedSeq++;
    }

    // Generate JSON output
    ofstream jsonFile("order_book.json");
    if (!jsonFile) {
        cerr << "Error opening JSON file for writing." << endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    jsonFile << "[";
    for (size_t i = 0; i < receivedPackets.size(); ++i) {
        Packet& packet = receivedPackets[i];
        jsonFile << "{";
        jsonFile << "\"symbol\":\"" << packet.symbol << "\",";
        jsonFile << "\"buysellindicator\":\"" << packet.buysellindicator << "\",";
        jsonFile << "\"quantity\":" << packet.quantity << ",";
        jsonFile << "\"price\":" << packet.price << ",";
        jsonFile << "\"packetSequence\":" << packet.packetSequence;
        jsonFile << "}";
        if (i < receivedPackets.size() - 1) {
            jsonFile << ",";
        }
    }
    jsonFile << "]";
    jsonFile.close();

    // Close the connection and clean up Winsock
    closesocket(sock);
    WSACleanup();

    return 0;
}
