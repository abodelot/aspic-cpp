#!/usr/bin/env python
#-*- coding:utf-8 -*-


import sys


class Statement:
	def __init__(self, instruction):
		self.instruction = instruction.strip() if instruction else None
		self.next = None

	def set_next(self, next):
		self.next = next

	def execute(self):
		print self.instruction
		return self.next

class ConditionnalStatement(Statement):
	def __init__(self, instruction):
		Statement.__init__(self, instruction)
		self.next_if_fail = None
		self.bypass_condition = False

	def set_next_if_fail(self, next):
		self.next_if_fail = next

	def execute(self):
		if not self.bypass_condition:

			print "[condition] " + self.instruction
			if raw_input(">> ") == "0":
				return self.next_if_fail

			if isinstance(self.next_if_fail, ConditionnalStatement):
				self.next_if_fail.bypass()
			return self.next

		self.bypass_condition = False
		return self.next_if_fail

	def bypass(self):
		self.bypass_condition = True
		if isinstance(self.next_if_fail, ConditionnalStatement):
			self.next_if_fail.bypass()

class WhileStatement(ConditionnalStatement):
	def __init__(self, instruction):
		ConditionnalStatement.__init__(self, instruction)


class DummyStatement(Statement):
	def __init__(self):
		Statement.__init__(self, "")

	def execute(self):
		return self.next

class ElseStatement(ConditionnalStatement):
	def __init__(self):
		ConditionnalStatement.__init__(self, None)

	def execute(self):
		if self.bypass_condition:
			self.bypass_condition = False
			return self.next_if_fail
		return self.next

class Parser:
	def __init__(self):

		self.start = None

	def parse(self, filename):
		stack = []
		previous = None

		for line in open(filename):

			first = line.split()[0]
			current = None

			if first == 'if':
				current = ConditionnalStatement(line)
				stack.append(current)

			elif first == 'elif':
				current = ConditionnalStatement(line)
				last = stack.pop()
				last.set_next_if_fail(current)
				stack.append(current)

			elif first == 'else':
				current = ElseStatement()
				last = stack.pop()
				last.set_next_if_fail(current)
				stack.append(current)

			elif first == 'end':
				current = DummyStatement()
				last = stack.pop()
				last.set_next_if_fail(current)
				if isinstance(last, WhileStatement):
					previous.set_next(last)

			elif first == 'while':
				current = WhileStatement(line)
				stack.append(current)
			else:
				current = Statement(line)



			if self.start is None:
				self.start = current

			if previous and previous.next == None:
					previous.set_next(current)

			previous = current


	def run(self):
		i = self.start.execute()
		while i:
			i = i.execute()

p = Parser()
p.parse(sys.argv[1])
p.run()
