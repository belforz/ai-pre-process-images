# Técnico de Pré-processamento de Imagens para Automatização

[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.x-green)](https://opencv.org/)



Uma ferramenta de pré-processamento de imagens projetada para curadoria automatizada e avaliação de qualidade de imagens. Esta aplicação em C++ processa imagens através de um pipeline abrangente que valida, corrige e otimiza imagens para automatização, dataset para treinamento de inteligência artificial como também parte do módulo de curadoria desenvolvido por mim em /belforz/photus-a e /photus-b. Para mais informações acesse os respectivos repositórios do projeto

## Funcionalidades

- **Validação de Imagens**: Verifica a integridade da imagem e suporte ao formato
- **Redimensionamento Automático**: Redimensiona imagens com base em critérios de resolução
- **Correção de Orientação EXIF**: Corrige automaticamente a orientação da imagem usando dados EXIF
- **Otimização de Compressão**: Aplica técnicas de compressão apropriadas
- **Geração de Miniaturas**: Cria miniaturas otimizadas
- **Conversão de Espaço de Cor**: Gera representações em múltiplos espaços de cor
- **Normalização**: Normaliza dados de imagem para consistência
- **Avaliação de Qualidade**: Detecta saturação, subexposição e problemas de desfoque
- **Extração de Metadados**: Extrai metadados abrangentes incluindo hash SHA-256
- **Saída JSON**: Fornece resultados detalhados de processamento em formato JSON

## Dependências

- OpenCV 4.x
- nlohmann/json (biblioteca JSON para C++)
- OpenSSL
- CMake 3.10 ou superior
- Compilador compatível com C++17

## Instalação

1. Clone o repositório:
   ```bash
   git clone <url-do-repositório>
   cd ai-pre-process-images
   ```

2. Instale as dependências:
   ```bash
   # Ubuntu/Debian
   sudo apt-get install libopencv-dev nlohmann-json3-dev libssl-dev cmake build-essential

   # macOS com Homebrew
   brew install opencv nlohmann-json openssl cmake

   # Ou compile a partir do código-fonte conforme necessário
   ```

3. Compile o projeto:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

   Ou use o script fornecido:
   ```bash
   ./scripts/build.sh
   ```

## Uso

### Uso Básico

Execute o executável com caminhos de arquivos de imagem:

```bash
./build/ai-preproccessor imagem1.jpg imagem2.png
```

### Com Orientação EXIF

Especifique valores de orientação EXIF para cada imagem:

```bash
./build/ai-preproccessor imagem1.jpg 3 imagem2.png 6
```

### Usando o Script Wrapper

```bash
./scripts/executable.sh imagem1.jpg 3 imagem2.png
```

### Tarefas de Compilação

Tarefas disponíveis no VS Code:
- **Clean Build**: `./scripts/clean.sh`
- **Build C++**: `./scripts/build.sh` (tarefa de compilação padrão)
- **Execute**: Executa o executável compilado
- **Build and Run**: Compila e executa com imagens de exemplo

## Formato de Saída

A aplicação gera metadados JSON para cada imagem processada:

```json
{
  "filename": "imagem.jpg",
  "hash": "hash-sha256",
  "width": 1920,
  "height": 1080,
  "aspect_ratio": 1.7778,
  "original_format": "JPEG",
  "image_type": "photo",
  "color_space": "BGR",
  "exif_orientation": "3",
  "source": "upload",
  "preproc_state": {
    "is_image_valid": true,
    "is_image_resized": true,
    "is_resolution_critic": false,
    "has_exif_data": true,
    "is_orientation_corrected": true,
    "has_compressed_image": true,
    "thumbnail_generated": true,
    "color_spaces_generated": true,
    "is_normalized": true,
    "has_satured_pixels": false,
    "has_underexposed_pixels": false,
    "original_filename": "imagem.jpg",
    "compression_type": "JPEG",
    "orientation_method": "EXIF",
    "saturated_pixel_count": 0,
    "underexposed_pixel_count": 0
  }
}
```

## Estrutura do Projeto

```
├── CMakeLists.txt          # Configuração de compilação
├── include/                # Arquivos de cabeçalho
│   ├── preproc/           # Módulos de pré-processamento
│   ├── connectors/        # Conectores de serviços externos
│   └── utils/             # Funções utilitárias
├── src/                   # Arquivos fonte
├── scripts/               # Scripts de compilação e execução
├── tests/                 # Testes unitários
├── images/                # Imagens de teste e amostras
├── build/                 # Saída de compilação (gerada)
└── README.md              # Este arquivo
```

## Testes

Execute testes individuais:

```bash
# Do diretório build
make test
```

Ou execute executáveis de teste específicos no diretório `tests/`.

## Licença

Licença MIT - veja o arquivo [LICENSE](LICENSE) para detalhes.

Copyright (c) 2025 Leo

## Contribuição

1. Faça um fork do repositório
2. Crie uma branch de funcionalidade
3. Faça suas alterações
4. Adicione testes se aplicável
5. Envie um pull request

## Suporte

Para problemas ou dúvidas, abra uma issue no repositório GitHub.

*Feito com ❤️ para processamento de imagens eficiente.*
