//
//  2019203102 유지성
//
//  main.cpp
//  subsetsum_dp
//
//  Created by Jisung Yu on 2023/05/05.
//

#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>

using namespace std;

void subset_sum(int count, vector<int> &num);
long long countSubset_dp(vector<int> &num, int target);
string saveSubset(vector<int> num);

vector<char> include;
long long subsetCount;
string subsets;
bool isPossible = true;
long long tempValue;

int main(int argc, const char * argv[]) {
    string line;
    int cnt;
    vector<int> counts;
    vector<vector<int>> numbers;
    
    while (getline(cin, line)) {    //숫자 입력받음
        if (line == "EOI") {
            break;
        }
        
        stringstream ss(line);
        
        ss >> cnt;
        counts.push_back(cnt);  // 숫자 갯수 저장
        vector<int> input(cnt);
        
        getline(cin, line);
        stringstream ss2(line);
        
        for (int i = 0; i < cnt; i++) { //입력받은 수 저장
            ss2 >> input[i];
        }
        
        sort(input.begin(), input.end());
        numbers.push_back(input);
    }
    
    for (int i = 0; i < numbers.size(); i++){
        subset_sum(counts[i], numbers[i]);
    }
    return 0;
}

void subset_sum(int count, vector<int> &num){
    double total = 0;
    subsetCount = 0;
    include.clear();    //include 초기화
    for (int i=0; i<num.size(); i++){
        total += (double)num[i];
        include.push_back('n');
    }
    
    if (total/2 - (int)(total/2) != 0)
        isPossible = false;
    else
        isPossible = true;
    
    int target = total/2;
    subsetCount = countSubset_dp(num, target);
    
    if (subsetCount/2 > ULONG_MAX - 1)
        cout << "NUMEROUS\n";
    else{
        if (subsetCount % 2 != 0)   //홀수갯수라는것은 공집합이 포함이 안된 것
            subsetCount += 1;
        cout << subsetCount/2 << endl;}
    cout << subsets << endl;
}

long long countSubset_dp(vector<int> &num, int target)  //수가 두배 되어서 나옴
{
    // 음수와 양수로 parsing
    vector<int> minusVec;
    vector<int> plusVec;
    int min = 0;
    int max = 0;
    int range = 0;
    for(int i=0; i<num.size(); i++){
        if(num[i] < 0)
            minusVec.push_back(num[i]);
        else
            plusVec.push_back(num[i]);
    }
    for(int i=0; i<minusVec.size(); i++)
        min+=minusVec[i];
    for(int i=0; i<plusVec.size(); i++)
        max+=plusVec[i];
    range = max - min + 1;  // min <= r <= max // 원하는 값 찾을때 min값 더해줘야함
    int newTarget = target - min;  // min값 생각해 정한 target
    
    vector<vector<long long>> dp(num.size()+1, vector<long long>(range, 0));// [배열 인덱스][범위]
    
    // target값 찾으면 dp[last index][target]으로 tracing, 최소값 찾을 경우 저 값이 true인지 보고 검색
    if (min >= 0 )  //집합에 음수가 없을경우, 공집합으로 공집합 하나 만들 수 있음
        dp[0][0] = 1;
    
    for (int i=1; i<=range; i++) {  // 공집합 인덱스 초기화
        dp[0][i] = 0;
    }
    
    if (min<0){
        for (int i=1; i<=num.size(); i++) { // 자기 자신의 경우 {공집합} {자기자신} 한 경우 있음
            dp[i][num[i-1]-min] = 1;
        }
    }
    
    for (int i=1; i<=num.size(); i++) {
        for (int j=0; j<=range; j++) {   //우선 배열 다 채움
            if (dp[i][j] > 2*(ULONG_MAX -1))
                return 2*ULONG_MAX;
            if (num[i-1] > j)    // j보다 인덱스가 크면 만들 수 없음
                dp[i][j] = dp[i-1][j];
            else {
                if(min<0 && num[i-1]==j+min)
                    dp[i][j] = 1 + dp[i-1][j-num[i-1]] + dp[i-1][j];  //자기자신과 공집합
                else{
                    int numTemp = j - num[i-1];
                    
                    if (numTemp >= range)   // range 값을 벗어나서 의도치 않은 오버플로우 발생할 때
                        dp[i][j] = dp[i-1][j];
                    else                    // 이전까지의 집합 갯수 + 현재 인덱스로 만들 수 있는 집합 갯수
                        dp[i][j] = dp[i-1][j-num[i-1]] + dp[i-1][j];
                }
            }
        }
    }
    if (!dp[num.size()][newTarget] || !isPossible){ //target 값이 원하지 않는 값 나오는걸 방지. ex) 2/1 == 0
        // subset sum 값이 0일때, sum 값이 최소가 되는걸 찾아야 함
        int k = (int)num.size(), l = newTarget + 1; // sum값이 홀수라 하면 target/2의 내림값이므로 + 1 해줬음. 그럼 target부터 시작.
        
        vector<char> copyInclude_1 = include;   // 테이블 기준 좌측, 우측 어느값이 더 작을지 모르니 비교
        vector<char> copyInclude_2 = include;
        
        // 작은범위
        for(int i=newTarget; i>0; i--){
            l = i;
            if (dp[num.size()][i]){
                while(k > 0 && l >= 0) {
                    if(dp[k-1][l]) {
                        k--;
                    }
                    else {
                        if (k != (int)num.size() && dp[k][l] == 0 && min<0)
                            break;
                        copyInclude_1[k-1] = 'y'; //include에 요소 저장
                        l = l - num[k-1];
                        k--;
                    }
                }
                break;
            }
        }
        // 큰범위
        for(int i=newTarget + 1; i<range; i++){
            l = i;
            if (dp[num.size()][i]){
                while(k > 0 && l >= 0) {
                    if(dp[k-1][l]) {
                        k--;
                    }
                    else {
                        if (k != (int)num.size() && dp[k][l] == 0 && min<0)
                            break;
                        copyInclude_2[k-1] = 'y'; //include에 요소 저장
                        l = l - num[k-1];
                        k--;
                    }
                }
                break;
            }
        }
        
        // 두 값 비교
        int ci1_y=0, ci2_y=0;
        int ci1_n=0, ci2_n=0;
        for(int i = 0; i < num.size(); i++) //copyInclude_1
            if(copyInclude_1[i]!='n'){
                ci1_y += num[i];
            }
        for(int i = 0; i < num.size(); i++)
            if(copyInclude_1[i]=='n'){
                ci1_n += num[i];
            }
        for(int i = 0; i < num.size(); i++) //copyInclude_2
            if(copyInclude_2[i]!='n'){
                ci2_y += num[i];
            }
        for(int i = 0; i < num.size(); i++)
            if(copyInclude_2[i]=='n'){
                ci2_n += num[i];
            }
        
        int total_1 = abs(ci1_y - ci1_n);
        int total_2 = abs(ci2_y - ci2_n);
        
        if (total_1<total_2)
            include = copyInclude_1;
        else
            include = copyInclude_2;
        
        subsets = saveSubset(num);
        return 0;
    }
    else{
        int k = (int)num.size(), l = newTarget;
        
        while(k > 0 && l >= 0) {    // 음수의 경우 l이 0일때 비어있는 수가 아니라 의미있는 수 이므로 포함
            if(dp[k-1][l]) {
                k--;
            }
            else {
                if (dp[k][l] == 0)
                    break;
                include[k-1] = 'y'; //include에 요소 저장
                l = l - num[k-1];
                k--;
            }
        }
        
    }
    
    subsets = saveSubset(num);
    return dp[num.size()][newTarget];
}

string saveSubset(vector<int> num){
    string str;
    str.append("{");
    int pValue1 = 0;
    int pValue2 = 0;
    
    int checkIn = 0;
    
    for(int i = 0; i < num.size(); i++)
        if(include[i]!='n'){
            str.append(to_string(num[i]));
            str.append(",");
            pValue1 += num[i];  // 집합 합
            checkIn += 1;
        }
    if (checkIn > 0)
        str.erase(str.end() - 1);
    str.append("}, {");
    
    checkIn = 0;
    for(int i = 0; i < num.size(); i++)
        if(include[i]=='n'){
            str.append(to_string(num[i]));
            str.append(",");
            pValue2 += num[i];  //집합 합
            checkIn +=1;
        }
    if (checkIn>0)
        str.erase(str.end() - 1);
    str.append("}");
    
    tempValue = abs(pValue1 - pValue2);    //절댓값 변환
    return str;
}
