#!/usr/bin/env python
# coding: utf-8

# In[1]:


import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set_style('darkgrid')


# In[2]:


# df = pd.read_csv('result.csv', names = ['bit', 'zeros'], header = None)
df = pd.read_csv('result_slightly_greedy.csv', names = ['zeros'], header = None)


# In[4]:


fig = plt.figure(figsize = (16, 9))
sns.lineplot(y = df.zeros, x = np.arange(1, df.zeros.shape[0] + 1), color = 'r')
plt.xticks(np.arange(1, df.zeros.shape[0] + 1, 1))
plt.yticks(np.arange(0, 33, 1))
# plt.xlim(0, 15)
plt.xlabel('Потужність множини бітів S')
plt.ylabel('Кількість початкових нулів у MDM-сигнатурі')
# plt.show()
plt.savefig('fig_slightly_greedy.png')


# In[ ]:




