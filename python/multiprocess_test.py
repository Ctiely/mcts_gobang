#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Oct 24 19:05:52 2018

@author: clytie
"""

from multiprocessing import Pool, Queue, Process

class DataGenerator(object):
    def __init__(self):
        self.data = []
        
    def generator(self, i, q):
        q.put(i)
        print("put %s" % i)
        


if __name__ == "__main__":
    q = Queue()
    generator = DataGenerator()
    procs = []

    for i in range(5):
        p = Process(target=generator.generator,
                args=(i, q))
        procs.append(p)
        p.start()

    # Collect all results into a single result dict. We know how many dicts
    # with results to expect.
    resultdict = []
    for i in range(5):
        resultdict.append(q.get())

    # Wait for all worker processes to finish
    for p in procs:
        p.join()
        
    print("done")
    print(resultdict)