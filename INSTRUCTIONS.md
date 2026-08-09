# Instruções: Caminho Dinâmico de Uploads e Categoria

Data: 2026-08-09

Este documento complementa o [README.md](README.md) e o
[NOTE_PREPROCESS_TRIGGER.md](NOTE_PREPROCESS_TRIGGER.md), detalhando o suporte
à pasta dinâmica de uploads e à categoria explícita gravada no
`local/images_index.json`.

## O que mudou

1. `pathsUtils.cpp` (`isValidPath`) passa a aceitar imagens dentro da pasta
   dinâmica de uploads, além de `images/`, do dataset local e de arquivos na
   raiz:

   ```
   /home/belforz/photus-system/data/uploads/{uuid}_{YYYYMMDD_HHMMSS}/nome_da_foto.formato
   ```

   `{uuid}_{YYYYMMDD_HHMMSS}` é o nome da pasta criada por upload; qualquer
   subpasta dentro de `.../uploads/` é aceita, sem necessidade de alterar o
   código a cada novo upload.

2. `runPreprocessingPipeline` e `updateImagesIndex` aceitam um parâmetro de
   `category` explícito. Quando informado, ele é gravado diretamente em
   `local/images_index.json` (campo `"category"`), substituindo a antiga
   inferência por regex sobre o caminho `dataset/<categoria>` — que não fazia
   sentido para a pasta de uploads.

3. `main.cpp` aceita a categoria de duas formas:
   - Modo manual: flag `--category <nome>`.
   - Modo trigger de IA: campo `category_code` do payload JSON (já existente,
     agora efetivamente repassado ao pipeline).

4. `scripts/run.sh` aceita `--dir <pasta>` e `--category <nome>` para
   varrer uma pasta de fotos (tipicamente a pasta dinâmica de uploads) e
   processar todas as imagens suportadas nela com a categoria informada.

## Como usar

### 1) Binário direto, uma ou mais imagens de uma pasta de upload

```bash
./build/ai-preproccessor --category retrato \
  "/home/belforz/photus-system/data/uploads/3fa1b2c4-.../foto1.jpg" \
  "/home/belforz/photus-system/data/uploads/3fa1b2c4-.../foto2.png" 3
```

### 2) Script `run.sh` varrendo a pasta inteira

```bash
./scripts/run.sh \
  --dir "/home/belforz/photus-system/data/uploads/3fa1b2c4-b1a2-4d3e-9f0a-1234567890ab_20260809_160000" \
  --category retrato
```

O script encontra todas as imagens suportadas (`jpg, jpeg, png, tiff, tif,
webp, bmp`) na pasta informada, de forma não recursiva, e chama o binário
para cada uma com `--category retrato`.

### 3) Trigger de IA (payload com `category_code`)

Continua funcionando como antes — a categoria do payload agora é usada
automaticamente:

```bash
./build/ai-preproccessor '{"category_code":"retrato"}' \
  "/home/belforz/photus-system/data/uploads/3fa1b2c4-.../foto1.jpg"
```

## Regras

1. Caminho de imagem deve estar em `images/`, no dataset local configurado
   em `pathsUtils.cpp`, na pasta dinâmica de uploads (`.../data/uploads/...`)
   ou ser um arquivo na raiz do projeto — caso contrário é rejeitado por
   `isValidPath`.
2. Categoria é opcional. Se omitida, o comportamento antigo é preservado:
   a primeira execução que ainda não tiver `"category"` no índice tenta
   inferir a categoria via regex `dataset/<categoria>` (só funciona para
   caminhos do dataset local); caso não consiga, usa `"unknown"`.
3. Quando uma categoria é passada explicitamente (via `--category` ou
   `category_code`), ela sempre sobrescreve o campo `"category"` do índice
   para a execução atual.

## Arquivos alterados

- `src/utils/pathsUtils.cpp`
- `include/preproc/PreProcessingPipeline.hpp`
- `src/preproc/PreProcessingPipeline.cpp`
- `src/main.cpp`
- `scripts/run.sh`

## Observação

O formato de saída do projeto (`ImageMetadata` / JSON por imagem em
`local/json/`) não foi alterado. Apenas a validação de caminho e o campo
`"category"` de `local/images_index.json` foram afetados.
