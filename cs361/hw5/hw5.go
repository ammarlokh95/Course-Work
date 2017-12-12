// hw5
package main

import (
	"fmt"
	"math/rand"
	"sync"
	"time"
)

type Employee struct {
	id        int
	prevStand string
}
type Customer struct {
	timeTicket int
}

func (emp *Employee) employees() {

	for {
		x := 1 + rand.Intn(4)
		for i := 0; i < x; i++ { // After helping x customers, employee goes for a break
			select {
			case <-quit: // if all customers are helped, return
				return
			case <-cellReg: // Helps customer at cell phone desk
				helpCell <- emp
			case <-tvReg: // Helps customers at home theatre desk
				helpTV <- emp
			case <-waitReg: // helps customers at the register
				helpReg <- emp
			}
		}
		emp.goOnBreak()
	}
}
func (emp *Employee) goOnBreak() {
	x := rand.Intn(5)
	fmt.Printf("employee %d going on break\n", emp.id)
	time.Sleep(time.Duration(x) * time.Second)
	defer fmt.Printf("employee %d back from break\n", emp.id)
}

var ticket = 1

func (cust *Customer) customer(enterStore chan object, wg *sync.WaitGroup) {
	obj := object{}
	cust.timeTicket = ticket
	ticket++
	enterStore <- obj
	fmt.Printf("customer %d has entered the store\n", cust.timeTicket)
	x := rand.Intn(3) // Randomly picks what desk to stand at
	switch x {
	case 0:
		//	fmt.Printf("customer %d waiting at register\n", cust.timeTicket)
		start := time.Now()
		waitReg <- cust.timeTicket
		emp := <-helpReg
		elapsed := time.Since(start)
		fmt.Printf("customer %d helped after %f s by employee %d at Register\n", cust.timeTicket, elapsed.Seconds(), emp.id)
	case 1:
		//	fmt.Printf("customer %d waiting at home theatre desk\n", cust.timeTicket)
		start := time.Now()
		tvReg <- cust.timeTicket
		emp := <-helpTV
		elapsed := time.Since(start)
		fmt.Printf("customer %d helped after %f s by employee %d at Home Theatre desk\n", cust.timeTicket, elapsed.Seconds(), emp.id)
	case 2:
		//	fmt.Printf("customer %d waiting at cell desk\n", cust.timeTicket)
		start := time.Now()
		cellReg <- cust.timeTicket
		emp := <-helpCell
		elapsed := time.Since(start)
		fmt.Printf("customer %d helped after %f s by employee %d at Cell Desk\n", cust.timeTicket, elapsed.Seconds(), emp.id)
	}
	<-enterStore
	wg.Done()
}

var e = 4
var r = 5
var s = 15
var c = 50
var waitReg = make(chan int, r)
var tvReg = make(chan int, 1)
var cellReg = make(chan int, 1)
var quit = make(chan int)
var helpReg = make(chan *Employee)
var helpTV = make(chan *Employee)
var helpCell = make(chan *Employee)

type object struct {
}

func main() {
	rand.Seed(time.Now().UTC().UnixNano())
	enterStore := make(chan object, s)

	var wg sync.WaitGroup
	wg.Add(c)
	for i := 0; i < e; i++ {
		emp := Employee{i + 1, ""}
		go emp.employees()
	}
	for i := 0; i < c; i++ {
		cus := Customer{}
		go cus.customer(enterStore, &wg)
	}
	wg.Wait()
	close(quit)
}
