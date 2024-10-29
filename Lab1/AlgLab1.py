# -*- coding: utf-8 -*-
"""AlgLab1.ipynb

Automatically generated by Colab.

Original file is located at
    https://colab.research.google.com/drive/1pXxIgIs-NQKIzJpuxnLj_M1c6mfWIYVi
"""

import random
import string

FILENAME = 'data'

def generate_data(num_of_rows, filename):
  with open(f"{filename}.txt", "w") as f:
    for i in range(num_of_rows):
      word_length = random.randint(1, 15)
      word_operation = random.choice('+-?')
      word_text = ''.join(random.choices(string.ascii_lowercase, k=word_length))
      f.write(f"{word_operation} {word_text}\n")
    f.write('#')

generate_data(10**6, FILENAME)

class Hasher:

  def __init__(self, p = 31, m=2**64):
    self.m = m
    self.p = p
    self.powers_m = self.get_powers(15, self.p, self.m)

  def get_powers(self, length, p, m):
    powers = []
    for i in range(length):
      powers.append(pow(p, i, m))
    return powers

  def get_hashes(self, text):
    hash = 0
    for i, char in enumerate(text):
      hash = (hash + self.powers_m[i] * ord(char)) % self.m
    return hash

  def check_subhashes(self, text1, text2):
    hash1, hash2 = 0, 0
    if len(text1) != len(text2): return False
    for i in range(len(text1)):
      hash1 = (hash1 + self.powers_m[i] * ord(text1[i])) % self.m
      hash2 = (hash2 + self.powers_m[i] * ord(text2[i])) % self.m
      if hash1 != hash2: return False
    return True

def add_word(word, structure, hasher):  # O(k)
  if len(structure) > 10**6:
    print('Max number of elements in database reached')
    return

  hash = hasher.get_hashes(word)
  structure.append({
      "word": word,
      "hash": hash,
  })
  return structure

def remove_word(word, structure, hasher):  # O(n+k)
  hash = hasher.get_hashes(word)
  return list(filter(lambda word_data:
                          not hasher.check_subhashes(word, word_data['word']),
                          structure))

def check_word(word, structure, hasher):  # O(n+k)
  hash = hasher.get_hashes(word)
  retrieved = next((word_data for word_data in structure
                    if hasher.check_subhashes(word, word_data['word'])), None)
  # if not retrieved:
  #   print(f'{word} - no')
  # else:
  #   print(f'{word} - yes')

def fill_structure(filename, hasher):  # O(n)
  structure = []

  with open(f"{filename}.txt", "r") as f:
    for row in f:

      if row == '#':
        # print('End of file')
        return structure

      splitted = row.split()
      operation, word = splitted[0], splitted[1]

      if not (len(word) <= 15 and word.islower() and word.isalpha()):
        print(f'Word {word} is invalid input data')
        continue

      if operation == '+':
        structure = add_word(word, structure, hasher)
      elif operation == '-':
        structure = remove_word(word, structure, hasher)
      elif operation == '?':
        check_word(word, structure, hasher)
      else:
        print(f"For word {word} invalid operation requested")

hasher = Hasher()
structure = fill_structure(FILENAME, hasher)

def get_counts(structure):
  unique_hashes = {}

  for element in structure:
    if element['hash'] in unique_hashes.keys(): # O(1)
      #unique_hashes[element['hash']][1] += 1
      unique_hashes.update({element['hash']: (unique_hashes[element['hash']][0], unique_hashes[element['hash']][1]+1)})
    else:
      unique_hashes[element['hash']] = (element['word'], 1)

  return [unique_hashes[key] for key in unique_hashes if unique_hashes[key][1] > 1]

result = get_counts(structure)
print(result)

print('\nPretty output')
for element in result:
  print(f'{element}')

#@title Check
from collections import Counter

words_array = [element['word'] for element in structure]
cnt = Counter(words_array)
print({x: count for x, count in cnt.items() if count > 1})

""" Оцінка часу виконання:

 Створення структури данних - **O(n)**, так як маємо цикл, який зчитує і обробляє кожен з рядків у вхідному файлі відповідно функціям add, remove, check

 Пошук повторюваних рядків і підрахунок їх кількості - **O(n)**
"""

#@title Check
import time
from matplotlib import pyplot as plt
import numpy as np

def fill(x):
  structure_1 = []
  hasher_1 = Hasher()
  for i in range(x):
    word_length = random.randint(1, 15)
    word_text = ''.join(random.choices(string.ascii_lowercase, k=word_length))
    add_word(word_text, structure_1, hasher_1)
  return structure_1

def get_y_values(x):
  structure = fill(int(x))

  start = time.time()

  get_counts(structure)

  end = time.time()
  length = end - start

  return length

x = np.linspace(10**3, 10**6, 100)
y = []

for i in x:
  y.append(get_y_values(i))

#plt.ylim(0, 0.00250)
plt.plot(x, y, 'go')
plt.xlabel("Number of rows")
plt.ylabel("Time (s)")

plt.show()
