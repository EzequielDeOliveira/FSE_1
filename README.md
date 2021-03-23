## Dados

| Aluno | Matrícula |
| --- | --- |
| Ezequiel De Oliveira Dos Reis | 16/0119316 |

## Dependências

* [WiringPi](http://wiringpi.com/)
* [ncurses](https://invisible-island.net/ncurses/announce.html)

## Execução

Para executar o programa basta clonar o repositório presente, entrar no diretório do repositório e executar os seguintes comandos

``` bash
# Para compilar o projeto
$ make

# Para executar o projeto
$ make run
```

### Resultado do experimento após 10 minutos


## Gŕafico de temperaturas em relação ao tempo

- Vermelho representa a temperatura externa
- Amarelo representa a temperatura de referência
- azul representa a temperatura interna

![image](https://user-images.githubusercontent.com/37127457/112221080-c56e5700-8c05-11eb-9524-a87090728c8b.png)

## Gŕafico com valor de acionamento dos autuadores

![image](https://user-images.githubusercontent.com/37127457/112221805-a4f2cc80-8c06-11eb-88dc-285c70767887.png)


## Referências

- [PID - Wikipedia](https://pt.wikipedia.org/wiki/Controlador_proporcional_integral_derivativo)  
- [Driver da Bosh para o sensor BME280](https://github.com/BoschSensortec/BME280_driver)  
- [Biblioteca BCM2835 - GPIO](http://www.airspayce.com/mikem/bcm2835/)  
- [Controle do LCD 16x2 em C](http://www.bristolwatch.com/rpi/i2clcd.htm)  
- [Biblioteca WiringPi GPIO](http://wiringpi.com)  
- [PWM via WiringPi](https://www.electronicwings.com/raspberry-pi/raspberry-pi-pwm-generation-using-python-and-c)

