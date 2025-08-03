// #include <mongocxx/client.hpp>
// #include <mongocxx/instance.hpp>
// #include <mongocxx/uri.hpp>
// #include <bsoncxx/json.hpp>
// #include <bsoncxx/builder/stream/document.hpp>
// #include "connectors/mongo.hpp"
// #include "MetadataSerializer.hpp"
// #include <cstdlib>
// #include <iostream>

// bool pingMongoDB() {
//     try {
//         const char* mongo_uri = std::getenv("MONGO_URI");
//         if (!mongo_uri) {
//             std::cerr << "ENVIRONMENT VARIABLE MONGO_URI NOT SET." << std::endl;
//             return false;
//         }
//         static mongocxx::instance instance{};
//         mongocxx::client client{mongocxx::uri{mongo_uri}};
//         auto db = client["admin"];
//         bsoncxx::builder::stream::document ping_cmd;
//         ping_cmd << "ping" << 1;
//         auto result = db.run_command(ping_cmd.view());
//         std::cout << "✅ MongoDB ping successful: " << bsoncxx::to_json(result) << std::endl;
//         return true;
//     } catch (const std::exception& e) {
//         std::cerr << "❌ MongoDB ping failed: " << e.what() << std::endl;
//         return false;
//     }
// }

// void sendMetadataToMongo(const ImageMetadata& metadata) {
//     try {
//         if (!pingMongoDB()) {
//             throw std::runtime_error("MongoDB connection test failed.");
//         }

//         const char* mongo_uri = std::getenv("MONGO_URI");
//         if (!mongo_uri) {
//             throw std::runtime_error("ENVIRONMENT VARIABLE MONGO_URI NOT SET.");
//         }

//         static mongocxx::instance instance{};
//         mongocxx::client client{mongocxx::uri{mongo_uri}};
//         auto db = client["mini-leandro"];
//         auto collection = db["ai-powered-data"];

//         bsoncxx::builder::stream::document document{};
//         document << "filename" << metadata.filename
//                  << "hash" << metadata.hash
//                  << "width" << metadata.width
//                  << "height" << metadata.height
//                  << "aspect_ratio" << metadata.aspect_ratio
//                  << "original_format" << metadata.original_format
//                  << "image_type" << metadata.image_type
//                  << "source" << metadata.source
//                  << "color_space" << metadata.color_space
//                  << "exif_orientation" << metadata.exif_orientation
//                  << "flags" << bsoncxx::builder::stream::open_document
//                      << "compressed" << metadata.preproc_state.has_compressed_image
//                      << "resolution_critical" << metadata.preproc_state.is_resolution_critic
//                      << "orientation_corrected" << metadata.preproc_state.is_orientation_corrected
//                      << "color_normalized" << metadata.preproc_state.is_normalized
//                      << "thumbnail_generated" << metadata.preproc_state.thumbnail_generated
//                  << bsoncxx::builder::stream::close_document;

//         auto result = collection.insert_one(document.view());
//         if (result) {
//             std::cout << "📦 Documento inserido com sucesso no MongoDB." << std::endl;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Erro ao salvar no MongoDB: " << e.what() << std::endl;
//     }
// }
