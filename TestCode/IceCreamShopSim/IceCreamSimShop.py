import threading
import random
import time

MAX_NUM_MANAGERS = 5
MAX_NUM_SERVERS = 10
MAX_NUM_CUSTOMERS = 5


class ThreadSafeVar(object):

	def __init__(self, value=None):
		self.lock = threading.Lock()
		self.value = value

	def set(self, value):
		with self.lock:
			self.value = value

	def get(self):
		var = None
		with self.lock:
			var = self.value
		return var


class Util(object):

	@classmethod
	def getRandBool(cls):
		return random.randint(0, 1)

	@classmethod
	def doRandDelay(cls):
		delay = random.randint(0,100)
		time.sleep(delay/1000.0)

	@classmethod
	def getTime(cls):
		return int(round(time.time() * 1000))


class Customer(object):

	def __init__(self, listOfServers, numCones=2):
		self.__numDesiredCones = numCones
		self.__listOfServers = listOfServers
		self.__conesCompletedSemi = threading.Semaphore(self.__numDesiredCones)

	def orderCones(self):
		for coneNum in range(self.__numDesiredCones):
			self.__orderSingleCone()

		for coneNum in range(self.__numDesiredCones):
			self.__conesCompletedSemi.acquire()


	def __orderSingleCone(self):
		Util.doRandDelay() # Decide on cone type
		server = self.__findFreeServer()
		threading.Thread(name='Validate Cone Callback',
						 target=server.makeCone,
						 args=(self.__conesCompletedSemi,)
						).start()

	def __findFreeServer(self):
		while True:
			for server in self.__listOfServers:
				if server.isFree():
					return server


class Employee(object):

	def __init__(self):
		self._busy = ThreadSafeVar(False)

	def isBusy(self):
		return self._busy.get()

	def isFree(self):
		return (not self.isBusy())


class Manager(Employee):

	def __init__(self):
		super(Manager, self).__init__()
		self.__requestedConeValidationLock = threading.Lock()

	def validateCone(self, callBack=None):
		with self.__requestedConeValidationLock:
			self._busy.set(True)
			Util.doRandDelay()
			passed = Util.getRandBool()
			if callBack:
				threading.Thread(name='Validate Cone Callback',
								 target=callBack,
								 args=(passed,)
								).start()
		self._busy.set(False)


class Server(Employee):

	def __init__(self, listOfManagers):
		super(Server, self).__init__()
		self.__coneMakingLock = threading.Lock()
		self.__listOfManagers = listOfManagers
		self.__customerDoneSemi = None

	def makeCone(self, doneSemi):
		self.__coneMakingLock.acquire()
		self._busy.set(True)
		self.__customerDoneSemi = doneSemi
		self.__createCone()
		# Will finish in self.__managersResponce

	def __createCone(self):
		Util.doRandDelay()
		manager = self.__findFreeManager()
		threading.Thread(name='Validate Cone',
						 target=manager.validateCone,
						 args=(self.__managersResponse,)
						).start()

	def __findFreeManager(self):
		while True:
			for manager in self.__listOfManagers:
				if manager.isFree():
					return manager

	def __managersResponse(self, passed):
		if passed:
			self._busy.set(False)
			self.__customerDoneSemi.release()
			self.__customerDoneSemi = None
		else:
			self.__createCone()


def main():
	listOfManagers = [Manager() for x in  range(random.randint(1, MAX_NUM_MANAGERS))]
	listOfServers = [Server(listOfManagers) for x in  range(random.randint(1, MAX_NUM_SERVERS))]
	listOfCustomers = [Customer(listOfServers) for x in  range(random.randint(1, MAX_NUM_CUSTOMERS))]
	st = Util.getTime()
	for customer in listOfCustomers:
		customer.orderCones()
	et = Util.getTime()

	print "Num Managers: %d, Num Servers: %d, Num Customers: %d, Time: %d" % (len(listOfManagers), len(listOfServers), len(listOfCustomers), et - st)

if __name__ == '__main__':
	main()
