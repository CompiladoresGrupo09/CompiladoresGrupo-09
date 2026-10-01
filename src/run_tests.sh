#!/usr/bin/env bash
# Suite de testes do interpretador.
#
#   tests/validos/*.c   devem terminar com codigo 0
#   tests/invalidos/*.c devem terminar com codigo diferente de 0
#
# Se existir <caso>.esperado ao lado do <caso>.c, a saida tambem e comparada:
# primeiro o stdout, depois o stderr (capturados separadamente para a ordem
# nao depender do buffer de cada sistema).
#
# Uso (de qualquer pasta, depois de rodar make):
#   bash run_tests.sh                     roda a suite e imprime o resumo
#   bash run_tests.sh --gerar <caso.c>    cria/atualiza o <caso>.esperado com a saida atual

cd "$(dirname "$0")" || exit 1

BIN=./interpretador
TESTS=../tests

if [ ! -f "$BIN" ] && [ ! -f "$BIN.exe" ]; then
    echo "Binario $BIN nao encontrado. Rode 'make' antes." >&2
    exit 1
fi

ERR_TMP=$(mktemp)
trap 'rm -f "$ERR_TMP"' EXIT

# Executa um caso e guarda em $saida (stdout seguido de stderr, sem \r) e $codigo
executar() {
    local out err
    out=$("$BIN" "$1" < /dev/null 2> "$ERR_TMP")
    codigo=$?
    err=$(cat "$ERR_TMP")
    if [ -n "$out" ] && [ -n "$err" ]; then
        saida=$(printf '%s\n%s' "$out" "$err" | tr -d '\r')
    else
        saida=$(printf '%s%s' "$out" "$err" | tr -d '\r')
    fi
}

if [ "$1" = "--gerar" ]; then
    if [ -z "$2" ] || [ ! -f "$2" ]; then
        echo "Uso: bash run_tests.sh --gerar <caminho/do/caso.c>" >&2
        exit 1
    fi
    executar "$2"
    printf '%s\n' "$saida" > "${2%.c}.esperado"
    echo "Gerado ${2%.c}.esperado (codigo de saida: $codigo)"
    exit 0
fi

passou=0
falhou=0
falhas=()

# $1 = arquivo .c, $2 = 1 se deve terminar com sucesso, 0 se deve falhar
verificar() {
    local arquivo=$1 deve_passar=$2
    local nome=${arquivo#"$TESTS"/}
    local esperado=${arquivo%.c}.esperado
    local motivo="" detalhe=""

    executar "$arquivo"

    if [ "$deve_passar" = 1 ] && [ "$codigo" -ne 0 ]; then
        motivo="esperado codigo 0, obtido $codigo"
        detalhe=$saida
    elif [ "$deve_passar" = 0 ] && [ "$codigo" -eq 0 ]; then
        motivo="esperado codigo diferente de 0, obtido 0"
        detalhe=$saida
    elif [ -f "$esperado" ] && [ "$saida" != "$(tr -d '\r' < "$esperado")" ]; then
        motivo="saida diferente de $(basename "$esperado")"
        if command -v diff > /dev/null; then
            detalhe=$(diff <(tr -d '\r' < "$esperado") <(printf '%s\n' "$saida"))
        else
            detalhe=$(printf -- '--- esperado:\n%s\n--- obtido:\n%s' "$(tr -d '\r' < "$esperado" | tail -n 8)" "$(printf '%s\n' "$saida" | tail -n 8)")
        fi
    fi

    if [ -z "$motivo" ]; then
        passou=$((passou + 1))
        if [ -f "$esperado" ]; then
            echo "[OK]    $nome (codigo e saida)"
        else
            echo "[OK]    $nome"
        fi
    else
        falhou=$((falhou + 1))
        falhas+=("$nome: $motivo")
        echo "[FALHA] $nome: $motivo"
        printf '%s\n' "$detalhe" | head -n 20 | sed 's/^/        /'
    fi
}

shopt -s nullglob

for f in "$TESTS"/validos/*.c; do
    verificar "$f" 1
done
for f in "$TESTS"/invalidos/*.c; do
    verificar "$f" 0
done

echo
echo "Resumo: $passou passaram, $falhou falharam (total $((passou + falhou)))"

if [ "$falhou" -gt 0 ]; then
    echo "Casos que falharam:"
    printf '  - %s\n' "${falhas[@]}"
    exit 1
fi
exit 0
