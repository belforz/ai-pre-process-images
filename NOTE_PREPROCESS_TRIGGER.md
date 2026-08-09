# Nota de Mudanca: Trigger do Pre-processador por category_code

Data: 2026-07-29

## O que mudou

Foi adicionado um novo modo de entrada no binario para acionar o pre-processamento a partir de um payload JSON.

Agora, o primeiro argumento pode ser:
- JSON bruto (string)
- Caminho para arquivo JSON

Quando o payload contem `category_code` (string nao vazia), o trigger do pre-processamento eh ativado automaticamente.

## Comportamento atual

- O roteamento automatico de diretorio por categoria ainda nao foi implementado (sera feito depois).
- Por enquanto, as imagens ainda devem ser passadas via argumentos CLI apos o payload.
- O modo antigo continua funcionando (sem payload, apenas imagens e EXIF opcional).

## Regras

1. Payload com `category_code` valido: executa o pre-processamento.
2. Payload sem `category_code`: encerra com aviso.
3. Payload valido, mas sem imagens apos ele: encerra com aviso.

## Exemplos

### 1) Payload JSON em linha + imagem

```bash
./build/ai-preproccessor '{"input_text":"mostre uma cena simples e cotidiana","category_code":"simplicidade"}' images/img_8bit.png
```

### 2) Arquivo JSON + imagem

```bash
./build/ai-preproccessor payload.json images/img_8bit.png
```

### 3) Modo antigo (sem payload)

```bash
./build/ai-preproccessor images/img_8bit.png
```

## Arquivo alterado na implementacao

- `src/main.cpp`

## Observacao tecnica

A validacao do argumento EXIF foi ajustada para parsing mais seguro de inteiro (evita aceitar valor parcialmente numerico).
