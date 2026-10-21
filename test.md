
# Tabela CIDR IPv4

| CIDR | Máscara de sub-rede | Bits de host | Total de IPs | Hosts utilizáveis | Incremento |
|---:|---|---:|---:|---:|---|
| /32 | 255.255.255.255 | 0 | 1 | 1* | 1 no 4º octeto |
| /31 | 255.255.255.254 | 1 | 2 | 2* | 2 no 4º octeto |
| /30 | 255.255.255.252 | 2 | 4 | 2 | 4 no 4º octeto |
| /29 | 255.255.255.248 | 3 | 8 | 6 | 8 no 4º octeto |
| /28 | 255.255.255.240 | 4 | 16 | 14 | 16 no 4º octeto |
| /27 | 255.255.255.224 | 5 | 32 | 30 | 32 no 4º octeto |
| /26 | 255.255.255.192 | 6 | 64 | 62 | 64 no 4º octeto |
| /25 | 255.255.255.128 | 7 | 128 | 126 | 128 no 4º octeto |
| /24 | 255.255.255.0 | 8 | 256 | 254 | 1 no 3º octeto |
| /23 | 255.255.254.0 | 9 | 512 | 510 | 2 no 3º octeto |
| /1 | 128.0.0.0 | 31 | 2.147.483.648 | 2.147.483.646 | 128 no 1º octeto |
| /0 | 0.0.0.0 | 32 | 4.294.967.296 | 4.294.967.294 | 256 no 1º octeto |

\* /32 representa um endereço individual. /31 permite dois endereços utilizáveis em enlaces ponto a ponto.

## Fórmulas importantes

- **Bits de host:** `32 - CIDR`
- **Total de IPs:** `2^(bits de host)`
- **Hosts utilizáveis:** `total de IPs - 2`, na regra tradicional.
- **Incremento:** `256 - valor do octeto parcial da máscara`.

## Como encontrar os endereços

Exemplo: `192.168.10.64/26`
  
| Informação | Resultado |
|---|---|
| Endereço de rede | 192.168.10.64 |
| Primeiro host | 192.168.10.65 |
| Último host | 192.168.10.126 |
| Broadcast | 192.168.10.127 |
| Próxima sub-rede | 192.168.10.128 |

No /26, o incremento é `256 - 192 = 64`. As sub-redes avançam de 64 em 64 no último octeto.
