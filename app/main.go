package main

import (
	"fmt"
	"gdg-devfest-demo/app/api"
	"os"
	"os/signal"
	"syscall"

	"github.com/TheThingsNetwork/ttn/mqtt"
)

const (
	appID     = "gdg-devfest-demo"
	accessKey = "YhU5w1qDZwmbKdPIA_dr5qKWA7WBs7POk2WWJeVjGgYa"
	broker    = "tcp://eu.thethings.network:1883"
)

func main() {
	client := mqtt.NewClient(nil, "devfest", appID, accessKey, broker)

	err := client.Connect()
	if err != nil {
		panic(err)
	}
	defer client.Disconnect()

	client.SubscribeUplink(func(_ mqtt.Client, _, devID string, message mqtt.UplinkMessage) {
		fmt.Printf("%s: ", devID)

		var measurement api.Measurement
		measurement.Unmarshal(message.PayloadRaw)
		fmt.Printf("Motion = %v; water = %d\n", measurement.GetMotion(), measurement.GetWater())
	})

	sigChan := make(chan os.Signal)
	signal.Notify(sigChan, os.Interrupt, syscall.SIGTERM)
	<-sigChan
}
