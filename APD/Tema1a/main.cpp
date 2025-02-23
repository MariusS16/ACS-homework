
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <set>
#include <unordered_map>
#include <vector>
using namespace std;

typedef struct {
  int thread_id;
  int map_threads;
  int reduce_threads;
  vector<string> files;
  vector<int> fileID;
  unordered_map<string, set<int>> *result;
  vector<unordered_map<string, set<int>>> *results;
  pthread_mutex_t *mutex;
  pthread_mutex_t *mutex2;
  pthread_mutex_t *mutex3;
  pthread_barrier_t *barrier;
} thread_arg_t;

string word_processing(string word) {
  string new_word = "";
  for (size_t i = 0; i < word.size(); i++) {
    if (isalpha(word[i])) {
      new_word += tolower(word[i]);
    }
  }
  return new_word;
}

void map(void *arg) {
  thread_arg_t *args = (thread_arg_t *)arg;
  for (size_t i = 0; i < args->files.size(); ++i) {
    string file = args->files[i];
    int fileID = args->fileID[i];
    ifstream filee(file);
    if (!filee.is_open()) {
      cerr << "Eroare la deschiderea fisierului " << file << endl;
      exit(1);
    }
    string word;
    while (filee >> word) {
      word = word_processing(word);
      if (word.empty()) {
        continue;
      }
      (*args->result)[word].insert(fileID);
    }
    filee.close();

    pthread_mutex_lock(args->mutex);
    for (auto &it : *args->result) {
      (*args->results)[it.first[0] - 'a'][it.first].insert(it.second.begin(),
                                                           it.second.end());
    }
    pthread_mutex_unlock(args->mutex);
  }
}

void reduce(void *arg) {
  thread_arg_t *args = (thread_arg_t *)arg;
  while (true) {
    pthread_mutex_lock(args->mutex2);
    char letter = 'a';
    bool done = true;
    for (int i = 0; i < 26; i++) {
      if (!(*args->results)[i].empty()) {
        letter = i + 'a';
        done = false;
        break;
      }
    }
    if (done) {
      pthread_mutex_unlock(args->mutex2);
      break;
    }

    unordered_map<string, set<int>> result = (*args->results)[letter - 'a'];
    (*args->results)[letter - 'a'].clear();
    pthread_mutex_unlock(args->mutex2);

    vector<pair<string, set<int>>> sorted_result(result.begin(), result.end());
    sort(sorted_result.begin(), sorted_result.end(),
         [](const pair<string, set<int>> &a, const pair<string, set<int>> &b) {
           if (a.second.size() != b.second.size()) {
             return a.second.size() > b.second.size();
           }
           return a.first < b.first;
         });

    string folder = "test_par/";
    if (args->map_threads == 1 && args->reduce_threads == 1) {
      folder = "test_sec/";
    }
    string filename = folder + letter + ".txt";
    ofstream outfile(filename);
    if (!outfile.is_open()) {
      cerr << "Eroare la deschiderea fisierului " << filename << endl;
      exit(1);
    }
    for (auto &it : sorted_result) {
      outfile << it.first << ":[";
      for (auto it2 = it.second.begin(); it2 != it.second.end(); ++it2) {
        if (it2 != it.second.begin()) {
          outfile << " ";
        }
        outfile << *it2;
      }
      outfile << "]";
      outfile << endl;
    }
    outfile.close();
  }
}

void *thread_function(void *arg) {
  thread_arg_t *args = (thread_arg_t *)arg;
  if (args->thread_id < args->map_threads) {
    map(args);
    pthread_barrier_wait(args->barrier);
  }

  if (args->thread_id >= args->map_threads) {
    pthread_barrier_wait(args->barrier);
    reduce(args);
  }
  pthread_exit(nullptr);
}

int main(int argc, char **argv) {
  if (argc < 4) {
    cerr << "Usage: ./tema1 <numar_mapperi> <numar_reduceri> <fisier_intrare>\n"
         << endl;
    return 1;
  }
  
  int M = atoi(argv[1]);
  int R = atoi(argv[2]);
  string file_name = argv[3];
  int num_threads = M + R;

  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
  pthread_barrier_t barrier;
  pthread_barrier_init(&barrier, NULL, num_threads);
  pthread_t tid[num_threads];
  thread_arg_t args[num_threads];
  int r;

  ifstream file(file_name);
  if (!file.is_open()) {
    cerr << "Eroare la deschiderea fisierului " << file_name << endl;
    return 1;
  }
  int n = 0;
  file >> n;
  vector<string> files(n);
  vector<int> fileIDs(n);
  for (int i = 0; i < n; i++) {
    string filee;
    file >> filee;
    files[i] = filee;
    fileIDs[i] = i + 1;
  }

  for (int i = 0; i < 26; i++) {
    string folder = "test_par/";
    if (M == 1 && R == 1) {
      folder = "test_sec/";
    }
    char letter = i + 'a';
    string filename = folder + letter + ".txt";
    ofstream outfile(filename);
    if (!outfile.is_open()) {
      cerr << "Eroare la deschiderea fisierului " << filename << endl;
      exit(1);
    }
  }

  unordered_map<string, set<int>> final_result;
  vector<unordered_map<string, set<int>>> results;
  results.resize(26);
  int files_per_thread = n / M;
  int rest = n % M;

  for (int i = 0; i < num_threads; i++) {
    int start = i * files_per_thread + min(i, rest);
    int end = start + files_per_thread + (i < rest ? 1 : 0);
    if (i < M) {
      args[i].files =
          vector<string>(files.begin() + start, files.begin() + end);
      args[i].fileID =
          vector<int>(fileIDs.begin() + start, fileIDs.begin() + end);
    }
    args[i].thread_id = i;
    args[i].result = new unordered_map<string, set<int>>;
    args[i].results = &results;
    args[i].mutex = &mutex;
    args[i].mutex2 = &mutex2;
    args[i].mutex3 = &mutex3;
    args[i].barrier = &barrier;
    args[i].map_threads = M;
    args[i].reduce_threads = R;
    r = pthread_create(&tid[i], NULL, thread_function, &args[i]);

    if (r) {
      cerr << "Eroare la crearea thread-ului " << i << endl;
      exit(-1);
    }
  }
  for (int i = 0; i < num_threads; i++) {
    r = pthread_join(tid[i], NULL);

    if (r) {
      cerr << "Eroare la asteptarea thread-ului " << i << endl;
      exit(-1);
    }
  }

  pthread_barrier_destroy(&barrier);
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&mutex2);
  pthread_mutex_destroy(&mutex3);
  return 0;
}