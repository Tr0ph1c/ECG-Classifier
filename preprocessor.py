import numpy as np
from scipy.signal import butter, filtfilt, firwin


def filter_ecg(ecg_signal: np.ndarray, fs: int) -> np.ndarray:
    hp_cutoff = 0.5
    hp_order = 2
    b_hp, a_hp = butter(hp_order, hp_cutoff / (fs / 2), btype='high', analog=False)
    ecg_hp = filtfilt(b_hp, a_hp, ecg_signal)

    lp_cutoff = 35  
    lp_order = 12
    fir_coeff = firwin(lp_order + 1, lp_cutoff / (fs / 2))
    ecg_filtered = filtfilt(fir_coeff, [1.0], ecg_hp)

    return ecg_filtered
