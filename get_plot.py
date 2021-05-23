#!/usr/bin/env python
# coding: utf-8

# In[38]:


import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
sns.set_style('darkgrid')


# In[39]:


df = pd.read_csv('result.csv', names = ['bit', 'zeros'], header = None)


# In[40]:


fig = plt.figure(figsize = (16, 9))
sns.lineplot(y = df.zeros, x = np.arange(1, df.zeros.shape[0] + 1), color = 'r')
plt.xticks(np.arange(1, df.zeros.shape[0] + 1, 1))
plt.yticks(np.arange(0, 21, 1))
# plt.xlim(0, 15)
plt.xlabel('Потужність множини бітів S')
plt.ylabel('Кількість початкових нулів у MDM-сигнатурі')
# plt.show()
plt.savefig('fig.png')


# In[ ]:




