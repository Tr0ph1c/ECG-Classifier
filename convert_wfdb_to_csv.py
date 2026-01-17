import wfdb
import pandas as pd
import numpy as np
from pathlib import Path

mitbih_base_path = 'mit-bih'

def record_path(record_name: str) -> str:
    return f'{mitbih_base_path}/{record_name}'

def peak_to_dict(signal_lead: np.ndarray, r_index: int, rr_interval: np.int16, label: str) -> dict:
    peak_dict = {}
    for i, val in enumerate(signal_lead[r_index-45:r_index+46]):
        peak_dict[f'samp_{i}'] = val
    peak_dict['rr_interval'] = rr_interval
    peak_dict['label'] = label
    
    return peak_dict

def get_rr_intervals(annon_samples: np.ndarray, fs: int) -> np.ndarray:
    rr_samples = np.diff(annon_samples)
    rr_intervals = rr_samples / fs * 1000  
    return rr_intervals

def is_beat_annotation(symbol: str) -> bool:
    beat_symbols = {'N', 'L', 'R', 'B', 'A', 'a', 'J', 'S', 'V', 'r', 'F', 'e', 'j', 'n', 'E', '/', 'f', 'Q', '?'}
    return symbol in beat_symbols

def is_file_exists(file_path: str) -> bool:
    try:
        with open(file_path, 'r'):
            return True
    except FileNotFoundError:
        return False
    
def shoud_skip_last_peak(last_r_signal_index, last_peak_index) -> bool:
    return last_r_signal_index + 45 > last_peak_index

def filter_beat_samples(annotations: list[str], samples: list[int]) -> list[int]:
    return [sample for ann, sample in zip(annotations, samples) if is_beat_annotation(ann)]

def get_record_peaks(record_name: str) -> pd.DataFrame:

    record = wfdb.rdrecord(record_path(record_name))
    record_annon = wfdb.rdann(record_path(record_name), 'atr', return_label_elements=['symbol', 'label_store'])
    
    r_samples = filter_beat_samples(record_annon.symbol, record_annon.sample)
    rr_intervals = get_rr_intervals(r_samples, record.fs)
    
    peaks_data = []
    max_itr = len(r_samples) - ( 1 if shoud_skip_last_peak(r_samples[-1], len(record.p_signal)) else 0)

    for i in range(1, max_itr):
        if is_beat_annotation(record_annon.symbol[i]):
            r_index = r_samples[i]
            rr_interval = rr_intervals[i-1]  
            label = record_annon.symbol[i]
            peak_dict = peak_to_dict(record.p_signal[:, 0], r_index, rr_interval, label)
            peaks_data.append(peak_dict)
    
    return pd.DataFrame(peaks_data)

def convert_wfdb_to_csv(output_csv: str, record_names: list[str]):
    all_peaks_df = pd.DataFrame()
    
    for record_name in record_names:
        record_peaks_df = get_record_peaks(record_name)
        all_peaks_df = pd.concat([all_peaks_df, record_peaks_df], ignore_index=True)
    
    all_peaks_df.to_csv(output_csv, index=False)

def is_numeric(s: str) -> bool:
    try:
        float(s)
        return True
    except ValueError:
        return False

def get_files_unique_names(directory_path):

    dir_path = Path(directory_path)

    unique_names = {file.stem for file in dir_path.iterdir() if file.is_file() and is_numeric(file.stem)}

    return list(unique_names)


if __name__ == '__main__':
    convert_wfdb_to_csv('mitbih_beats.csv', get_files_unique_names(mitbih_base_path))